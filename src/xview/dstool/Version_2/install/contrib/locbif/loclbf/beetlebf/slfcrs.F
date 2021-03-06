C  -------------------------------------------------------------------
C
C	Copyright (1990-1995) by Alexander Khibnik, Yuri Kuznetsov, and 
C	Eugene Nikolaev.
C
C The Locbif computation code in DsTool is distributed in the hope that it  
C will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty  
C of FITNESS FOR A PARTICULAR PURPOSE.  The software is provided as is without 
C any obligation on the part of Locbif authors, Cornell faculty, 
C Cornell staff or Cornell students to assist in its use, correction, 
C modification or enhancement. 
C
C  -------------------------------------------------------------------

      SUBROUTINE SLFCRS (ISTEP,NX,NDIMM,NG,NCAN,ICRS,IPRCRS,STX2,EPS,
     A                   FUN,DFDX,OUT,DFUN,ISTOP)
C     3.05.90�
C     6.08.1991
C     26.08.1991
C     13.11.92
C     15.06.94
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA  SLFCRS  �PE�HA�HA�EHA ��� �TO�HEH�� TO�EK             *
C*    CAMO�EPECE�EH�� KP�BO� B NX-MEPHOM �POCTAHCTBE.                  *
C*    TO�KA CAMO�EPECE�EH�� �PO��EHA, EC�� O�PE�E��TE�� MATP���        *
C*    �ACTH�X �PO��BO�H�X COOTBETCTB����� ��KC�POBAHHO� �EPEMEHHO�     *
C*    MEH�ET �HAK. HO�� O�EPE�E��TE�� �TO�H�ETC� METO�OM CEK���X.      *
C*                                                                     *
C*    BXO�H�E � A P A M E T P �:                                       *
C*    --------------------------                                       *
C*    ISTEP -��C�O �A�OB, �PO��EH�X B�O�� KP�BO�.                      *
C*    NX    -��C�O �EPEMEHH�X (X(1),...,X(NX)).                        *
C*    NDIM  -�A�B�EHHA� CTOP�HA� PA�MEPHOCT� MATP��� DFUN(NDIM,NX).    *
C*    NG    -HOMEP B��E�EHHO� �EPEMEHHO�.                              *
C*    NCAN  -HOMEP KAHA�A ��� B�BO�A �POTOKO�A �TO�HEH�� TO�K� CAMO�EP.*
C*    ICRS  -�APAMETP, �KA��BA���� HA HEO�XO��MOCT� �TO�HEH�� TO�K�    *
C*           CAMO�EPECE�EH��:                                          *
C*             0 - TO�KA CAMO�EPECE�EH�� HE �TO�H�ETC�;                *
C*             1 - TO�KA CAMO�EPECE�EH�� �TO�H�ETC�.                   *
C*    IPRCRS-�APAMETP, �KA��BA���� CTE�EH� �O�PO�HOCT� �POTOKO�A:      *
C*            -1 - �POTOKO� OTC�CTB�ET;                                *
C*             0 - B��A�TC� KOOP��HAT� TO�K� CAMO�EPECE�EH��;          *
C*             1 - B��AETC� HEKOTOPA� �H�OPMA��� O� �TEPA���X.         *
C*    STX2  -KOOP��HAT� �OC�E�HE� TO�K� HA KP�BO�;                     *
C*    EPS   -TO�HOCT� O�PE�E�EH�� TO�K� HA KP�BO� B METO�E H��TOHA.    *
C*    FUN   -�M� �O��PO�PAMM� B���C�EH�� �PAB�X �ACTE�.                *
C*    DFDX  -�M� �O��PO�PAMM� B���C�EH�� MATP��� �ACTH�X �PO��BO�H�X   *
C*           �A�AHHO� �BH�M O�PA�OM.                                   *
C*    DFUN  -�B�MEPH�� MACC�B PA�MEPHOCT� DFUN(NDIM,NX),               *
C*           CO�EP�A��� MATP��� �ACTH�X �PO��BO�H�X.                   *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*   �P� ��MEHEH�� MAKC�MA��HO�O ��C�A  NDIM �EPEMEHH�XHEO�XO��MO*     *
C*   �C�PAB�T� PA�MEPHOCT� MACC�BOB:                                   *
C*   STX1(*) STX2(*) X1(*) X2(*) XCRS(*) XABD(*) VMOVE(*) 	       *
C*   FNCRS(*) VMOVE3(*)                                                *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2 (I-N)
      include 'beetlebf.dim'
      EXTERNAL FUN, DFDX, OUT
      DIMENSION STX1(NDIM), STX2(1), X1(NDIM), X2(NDIM), XCRS(NDIM),
     A          XABD(NDIM), DFUN(NDIM,NX), 
     B          FNCRS(NDIM), VMOVE3(NDIM), INDEX(NPTYP)
      DIMENSION ITNWTZ(10)
C
      COMMON /ADD2B/ EPSCRS
C
      COMMON /OUT3/ EPSCR, EPSR, DETZ, CONDZ
      COMMON /OUT7/ INDC
C
      COMMON /LEQV1/  DETL, CONDL
      COMMON /CORR1/  DET,  COND, SGNDET
C
      COMMON /WRTO1/ NFIX1, NFIX
      COMMON /WRTO2/ DMOVE1, DMOVE
      COMMON /WRTO4/ EPSC, EPSN
      COMMON /WRTO6/ ITC,IT2
      COMMON /WRTO7/ VMOVE(NDIM),VMOVN(NDIM)
      SAVE DETOLD,CONDST,STX1

      ISTOP = 0
      IF (ICRS .EQ. 0) RETURN
      DET2       = DETL
      DET1       = DETOLD
      DETOLD     = DET
      COND2      = CONDL
      COND1      = CONDST
      CONDST     = COND
C
C.... KOHTPO�� � �TO�HEH�E TO�K� CAMO�EPECE�EH��.
C
c      WRITE (6,*) ' DET=',DET,'  DETL=',DETL,'  DETOLD=',DETOLD 
         IF (ISTEP .EQ. 0) GOTO 370
            DO 200 I=1,NX
               X1(I)   = STX1(I)
               X2(I)   = STX2(I)
c              STX1(I) = STX2(I)
 200        CONTINUE
            IF (DET1*DET2 .GT. 0.D0) GOTO 360
c                IF (DABS(DET2).LT.0.01*EPSCRS
c     A              .AND.DABS(DET1).LT.0.01*EPSCRS) GOTO 360
                 DF = DABS((DET2-DET1)/(X1(NFIX1)-X2(NFIX1)))
                 IF (DF.LT.0.1*EPSCRS) GOTO 360
c
C
C.... �O��OTOBKA K �TEPA���M �TO�HEH�� TO�K� CAMO�EPECE�EH��.
C
               DETSTP = 5.D-06
               DO 100 I=1,4
                  INDEX(I) = 0
 100           CONTINUE
               INDC   = 0
               ITCREM = ITC
               ITC    = 5*ITC
               ITMAX  = 10
               EPSC   = EPS
	       EPSN   = 0.D0
               EPSCR  = EPSCRS
	       INDLOC = 0
C
C.... �HA�EH�� DETL,CONDL,DET,COND-�OPT�TC� B �PO�ECCE �TEPA���
C     �O H��TOH�, �O�TOM� �X HEO�XO��MO �A�OMH�T�.
C
               ITNWTZ(1)  = 0
C
C.... ��K� �TO�HEH�� TO�K� CAMO�EPECE�EH��.
C
               DO 340 ITER =1,ITMAX
C
C.... �PO�HO� TO�K� CAMO�EPECE�EH��.
C
 205 		  CONTINUE
                  IF (INDLOC .EQ. 0) RX = DET2/(DET2-DET1)
                  IF (INDLOC .EQ. 1) RX = 0.5D0
                  IF (INDLOC .EQ. 2) RX = 4.D0/9.D0
C
                  DO 210 I=1,NX
                     XCRS(I) = X2(I)-(X2(I)-X1(I))*RX
 210              CONTINUE
                  ITEND = ITC
                  IPR   = 0
c      PRINT *,' SLFCRS:'
c      PRINT *,'  ITER=',ITER,'  INDLOC=',INDLOC
c      PRINT *,'   DET1=',DET1,'  DET2=',DET2
c      PRINT *,'   X1=',(X1(J), J=1,NX)
c      PRINT *,'   X2=',(X2(J), J=1,NX)
c      PRINT *,'   XC=',(XCRS(J), J=1,NX)
C
C.... �TO�HEH�E TO�K� HA KP�BO� METO�OM H��TOHA.
C
                  IER = 0
                  CALL NEWTN (NX,NDIM,NG,NFIX1,NCAN,XCRS,EPSC,EPSN,
     A                        ITEND,IPR,FUN,DFDX,DFUN,IER,ISTOP)
                  IF (ISTOP .NE. 0) RETURN
                  ITNWTZ(ITER) = ITEND
c      PRINT *,'   IER=',IER
c      PRINT *,'   XC=',(XCRS(J), J=1,NX)
                  IF (IER   .EQ. 0) GOTO 280
C
                  INDLOC=INDLOC+1
                  IF (INDLOC .LE. 2) GOTO 205
C
C.... HE CO���C� H��TOHOBCK�E �TEPA���.
C
 215              CONTINUE  
                     IWRT         = 1 - IER
                     IPRTCL       = 3
                     DO 220 I=1,NX
                        XABD(I) = XCRS(I)
 220                 CONTINUE
C
C.... B��EPEM TO�K� HA KP�BO�, ����A���� K TO�KE CAMO�EPECE�EH��.
C
                     IF (DABS(DET1) .GT. DABS(DET2)) GOTO 240
                        DETZ   = DET1
                        CONDZ  = COND1
                        DO 230 I=1,NX
                           XCRS(I) = X1(I)
 230                    CONTINUE
                        GOTO 260
 240                 CONTINUE
                        DETZ   = DET2
                        CONDZ  = COND2
                        DO 250 I=1,NX
                           XCRS(I) = X2(I)
 250                    CONTINUE
 260                 CONTINUE
C
C.... KOHTPO�� �OCT��H�TO� TO�HOCT� B TO�KE, ����A��E� K TO�KE
C     CAMO�EPECE�EH��.
C
                      EPSR = DABS(X2(NFIX1)-X1(NFIX1))
C
C.... �TEPA��� �AKOH�EH�-HE CO���C� �TEPA��� H��TOHA.
C
                     DMOVER= DMOVE
                     DMOVE = DMOVE1
                     NFIXR = NFIX
                     NFIX  = NFIX1
#if defined(hib)
                     CALL WRTCRS (IPRTCL,IWRT,IPRCRS,ITER,ITNWTZ,ITEND,
     A                            NCAN,NX,NDIM,EPSCR,EPSR,DETZ,CONDZ,
     B                            XCRS,XABD,DFUN)
#endif
                     DMOVE = DMOVER
                     NFIX  = NFIXR
                     INDC  =-1
                     GOTO 355
 280              CONTINUE
C
        	R1 = 0.D0
		R2 = 0.D0
		DO 285 I=1,NX
                   R1 = R1+(XCRS(I)-STX1(I))*(STX2(I)-STX1(I))
                   R2 = R2+(XCRS(I)-STX2(I))*(STX2(I)-STX1(I))
 285            CONTINUE
                IF (R1*R2 .GT. 0.) GOTO 215
C
C.... �A�OMH�M HOBOE �HA�EH�E O�PE�E��TE�� (DETL).
C
                        IER = 0
                        CALL TANGNT (NX,NDIM,NFIX1,XCRS,FNCRS,VMOVE3,
     A                               FUN,DFDX,DFUN,IER,ISTOP)
                        IF (ISTOP .NE. 0) RETURN
c      PRINT *,'   IER (TANGNT)=',IER
                        IF (IER .NE. 0 .AND. IER .NE. -1) GOTO 215
                  DETZ         = DETL
                  CONDZ        = CONDL
C
C.... B��OP H��HO�O �HTEPBA�A C H��EM  DETZ = 0.
C
                  IF (DETZ*DET1 .LT. 0.D0) GOTO 300
                      DET1   = DETZ
                      COND1  = CONDZ
                      DO 290 I=1,NX
                         X1(I) = XCRS(I)
 290                  CONTINUE
                      GOTO 320
 300              CONTINUE
                      DET2   = DETZ
                      COND2  = CONDZ
                      DO 310 I=1,NX
                         X2(I) = XCRS(I)
 310                  CONTINUE
 320              CONTINUE
C
C.... KOHTPO�� �OCT��H�TO� TO�HOCT�.
C
                  EPSR = DABS(X2(NFIX1)-X1(NFIX1))
                  DF = DABS((DET1-DET2)/(X1(NFIX1)-X2(NFIX1)))
                  FN=    DABS (DETZ/DF)
                  IF (EPSR.LE.EPSCR .OR. FN.LE.0.1*EPSCR) GOTO 350
 340           CONTINUE
C
C.... �A�AHHA� TO�HOCT� HE �OCT��H�TA.
C
               INDC   =-1
               IPRTCL = 2
               ITER   = ITMAX
               GOTO 352
 350           CONTINUE
C
C.... TO�KA CAMO�EPECE�EH�� �TO�HEHA C �A�AHHO� TO�HOCT��.
C
               IPRTCL = 1
 352           CONTINUE
#if defined(hib)
c               CALL WRTCRS (IPRTCL,IWRT,IPRCRS,ITER,ITNWTZ,ITEND,NCAN,
c     A                      NX,NDIM,EPSCR,EPSR,DETZ,CONDZ,XCRS,XABD,
c     B                      DFUN)
#endif
 355           CONTINUE
C
C.... O�PA�OTKA �TO�HEHHO� TO�K� CAMO�EPECE�EH�� �O���OBATE�EM.
C
               INDEX(2) = 1
               CALL OUT (ISTEP,INDEX,NX,NDIM,XCRS,VMOVE,DFUN,ISTOP)
               ITC    = ITCREM
               IF (ISTOP .NE. 0) RETURN
 360        CONTINUE
 370     CONTINUE
C
C.... �O��OTOBKA K �EPBOM� �A�� B�O�� KP�BO�.
C
         DO 380 I=1,NX
            STX1(I) = STX2(I)
 380     CONTINUE
         RETURN
         END
