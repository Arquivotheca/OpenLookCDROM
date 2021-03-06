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

      SUBROUTINE EXTR (ISTEP,NX,NDIMM,NG,NCAN,NUE,IVARS,IPRE,STX2,EPS,
     A                 FUN,DFDX,OUT,DFUN,ISTOP)
C     3.05.90�.
C     5.08.1991
C     13.11.92
C     15.06.94
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA  EXTR  �PE�HA�HA�EHA ��� �TO�HEH�� TO�EK �KCTPEM�MA    *
C*    �A�AHH�X �EPEMEHH�X HA KP�BO�. C��TAETC�, �TO �EPEMEHHA�         *
C*    �OCT���A CBOE�O �KCTPEMA��HO�O �HA�EH��, EC�� COOTBETCTB���A�    *
C*    KOM�OHEHTA KACATE��HO�O BEKTOPA O�PAT��AC� B HO��.               *
C*    � A P A M E T P �:                                               *
C*    ==================                                               *
C*    ISTEP -��C�O �PO�E�AHH�X �A�OB B�O�� KP�BO�.                     *
C*    NX    -PA�MEPHOCT� �POCTPAHCTBA �EPEMEHH�X.                      *
C*    NDIM  -�A�B�EHHA� CTO�HA� PA�MEPHOCT� MATP��� DFUN.              *
C*    NG    -HOMEP B��E�EHHO� �EPEMEHHO�.                              *
C*    NCAN  -HOMEP KAHA�A B�BO�A PE����TATOB.                          *
C*    NUE   -��C�O �EPEMEHH�X, AHA����P�EM�X HA �KCTPEM�M.             *
C*    IVARS -MACC�B HOMEPOB AHA����P�EM�X �EPEMEHH�X.                  *
C*    STX2  -KOOP��HAT� TEK��E� TO�K� HA KP�BO�.                       *
C*    EPS   -TO�HOCT� METO�A H��TOHA.                                  *
C*    IPRE  -�P��HAK �O�PO�HOCT� �E�AT� (CM.O��CAH�E �APAMETPOB BEETLE.*
C*    FUN   -�M� �PO�PAMM� B���C�EH�� BEKTOP-��HK���.                  *
C*    DFDX  -�M� �PO�PAMM� B���C�EH�� MATP��� �KO�� �O �BH�M �OPM��AM. *
C*    DFUN  -MACC�B, CO�EP�A��� MATP��� �ACTH�X �PO��BO�H�X.           *
C*    IER   -�P��HAK OKOH�AH�� �PO�PAMM� (�OKA H� HA �TO HE B���ET).   *
C*    ISTOP -�PEP�BAH�E PA�OT� �PO�PAMM� �O���OBATE�EM.                *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*    �P� ��MEHEH�� MAKC�MA��HO�O ��C�A �EPEMEHH�X-NDIM, HEO�XO��MO    *
C*    ��MEH�T� PA�MEPHOCT� MACC�BOB:                                   *
C*    XEXT(*) STX1(*) STV1(*) XABD(*) X1(*) X2(*) VMOVE1(*) VMOVE2(*)  *
C*    VMOVE(*)        STV2(*)                                          *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2 (I-N)
      include 'beetlebf.dim'
      EXTERNAL   FUN, DFDX, OUT
      DIMENSION  XEXT(NDIM), STX1(NDIM), STX2(1), STV1(NDIM),
     A           DFUN(NDIM,NX), IVARS(1), XABD(NDIM), X1(NDIM),
     B           X2(NDIM), INDEX(NPTYP), VMOVE1(NDIM),
     C           VMOVE2(NDIM), VMOVE3(NDIM), FNEXT(NDIM)
C
      COMMON /ADD4B/ EPSEXT
C
      COMMON /OUT5/  EPSE,  EPSR
      COMMON /OUT6/  INDE
C
      COMMON /WRTO1/ NFIX1, NFIX
      COMMON /WRTO2/ DMOVE2, DMOVE
      COMMON /WRTO4/ EPSC, EPSN
      COMMON /WRTO6/ ITC,IT2
      COMMON /WRTO7/ STV2(NDIM),VMOVN(NDIM)
      SAVE DMOVE1,STX1,STV1,/OUT5/,/OUT6/
C
      ISTOP = 0
      IF (NUE .EQ. 0) RETURN
      IF (ISTEP .EQ. 0) GOTO 400
          DO 100 I=1,NPTYP
             INDEX(I) = 0
 100      CONTINUE
          EPSC  = EPS
          EPSE  = EPSEXT
          ITMAX = 10
          DONE1 = DMOVE1/DABS(DMOVE1)
          DONE2 = DMOVE2/DABS(DMOVE2)
C
C.... ��K� �POBEPK� HA �KCTPEM�M BCEX �EPEMEHH�X.
C     KOOP��HATA  XEXT(NFIX1)  HE �POBEP�ETC�.
C
          DO 390 IVAR = 1,NUE
            INDE = 0
            NEXT = IVARS(IVAR)
            IF (NEXT .EQ. NFIX1) GOTO 380
C
C.... �O��OTOBKA PA�O��X MACC�BOB.
C
                DO 200 I = 1,NX
                  X1(I)     = STX1(I)
                  X2(I)     = STX2(I)
                  VMOVE1(I) = STV1(I)
                  VMOVE2(I) = STV2(I)
 200            CONTINUE
C
C.... �POBEPKA �C�OB�� �KCTPEM�MA.
C
                FEXT1 = DONE1*VMOVE1(NEXT)
                FEXT2 = DONE2*VMOVE2(NEXT)
                IF (FEXT1*FEXT2 .GT. 0.D0) GOTO 370
                DF = DABS((FEXT1-FEXT2)/(X1(NFIX1)-X2(NFIX1)))
                IF (DF.LT.0.1*EPSE) GOTO 370
		INFLOC = 0
C
C.... O�PE�E�EH�E XAPAKTEPA �KCTPEM�MA:
C                   IEXT = 1 -�OCT��AETC� MAKC�M�M;
C                   IEXT =-1 -�OCT��AETC� M�H�M�M;
C
                    IEXT = 1
                    IF (FEXT1.LE.0.D0 .OR. FEXT2.GE.0.D0) IEXT =-1
C
C.... ��K� �TO�HEH�� KOOP��HAT �KCTPEM�MA  XEXT(NEXT).
C
                    DO 350 ITER=1,ITMAX
C
C.... �PO�HO� H��� KOM�OHEHT� VMOVE(NEXT) KACATE��HO�O BEKTOPA.
C
 205              CONTINUE
                  IF (INDLOC .EQ. 0) RX = FEXT2/(FEXT2-FEXT1)
                  IF (INDLOC .EQ. 1) RX = 0.5D0
                  IF (INDLOC .EQ. 2) RX = 4.D0/9.D0
C
                       DO 210 I=1,NX
                          XEXT(I) = X2(I)-(X2(I)-X1(I))*RX
 210                   CONTINUE
                       IT = ITC
                       IPR= 0
                       IER= 0
                       CALL NEWTN (NX,NDIM,NG,NFIX1,NCAN,XEXT,EPS,EPSN,
     A                             IT,IPR,FUN,DFDX,DFUN,IER,ISTOP)
                       IF (ISTOP .NE. 0) RETURN
                       IF (IER   .EQ. 0) GOTO 220
                        INDLOC=INDLOC+1
                        IF (INDLOC .LE. 2) GOTO 205
C
C.... HE CO���C� H��TOHOBCK�E �TEPA���:�TO�HEH�E TO�K� �KCTPEM�MA
C     �PEKPA�AETC�.
C
 215                    CONTINUE
                           IWRT  = 1 - IER
                           GOTO 230
 220                    CONTINUE
        	R1 = 0.D0
		R2 = 0.D0
		DO 225 I=1,NX
                   R1 = R1+(XEXT(I)-STX1(I))*(STX2(I)-STX1(I))
                   R2 = R2+(XEXT(I)-STX2(I))*(STX2(I)-STX1(I))
 225            CONTINUE
                IF (R1*R2 .GT. 0.) GOTO 215
C
                        IER = 0
                        CALL TANGNT (NX,NDIM,NFIX1,XEXT,FNEXT,VMOVE3,
     A                               FUN,DFDX,DFUN,IER,ISTOP)
                        IF (ISTOP .NE. 0) RETURN
                        IF (IER   .EQ. 0) GOTO 290
                        INDLOC=INDLOC+1
                        IF (INDLOC .LE. 2) GOTO 205
C
C.... HE HA��EH KACATE��H�� BEKTOP:�TO�HEH�E TO�K� �KCTPEM�MA
C     �PEKPA�AETC�.
C
                            IWRT = 5 - IER
 230                    CONTINUE
                            DO 235 I=1,NX
                               XABD(I) = XEXT(I)
 235                        CONTINUE
C
C.... B��EPEM TO�K� HA KP�BO�, ����A���� K TO�KE �KCTPEM�MA.
C
                            IF (DABS(FEXT1) .GT. DABS(FEXT2)) GOTO 250
                                FEXT3 = FEXT1
                                DO 240 I=1,NX
                                   XEXT(I)   = X1(I)
                                   VMOVE3(I) = VMOVE1(I)
 240                            CONTINUE
                                GOTO 270
 250                        CONTINUE
                                FEXT3 = FEXT2
                                DO 260 I=1,NX
                                   XEXT(I)   = X2(I)
                                   VMOVE3(I) = VMOVE2(I)
 260                            CONTINUE
 270                        CONTINUE
C
C.... KOHTPO�� TO�HOCT� B TO�KE, ����A��E� K TO�KE �KCTPEM�MA.
C
                             EPSR = DABS(X2(NFIX1)-X1(NFIX1))
C
C.... �O��OTOB�M �AHH�E ��� �PO�PAMM�   WRTOUT.
C
                           DMOVER = DMOVE
                           NFIXR  = NFIX
                           DMOVE  = DMOVE1
                           NFIX   = NFIX1
                           IPRTCL = 3
#if defined(hib)
                           CALL WRTEXT (IPRTCL,IWRT,IPRE,ITER,IT,
     A                                  NCAN,NX,NDIM,NEXT,IEXT,EPSE,
     B                                  EPSR,XEXT,XABD,VMOVE3,DFUN)
#endif
                           DMOVE = DMOVER
                           NFIX  = NFIXR
                           INDE  =-1
                           GOTO 365
 290                    CONTINUE
C
C.... �O��PAB�M �TO�H�EM�� KOM�OHEHT� KACATE��HO�O BEKTOPA.
C
                        FEXT3 = DONE1*VMOVE3(NEXT)
C
C.... B��OP TO�EK, ����A���X K TO�KE �KCTPEM�MA.
C
                        IF (FEXT1*FEXT3 .LT. 0.D0)  GOTO 310
                           FEXT1 = FEXT3
                           DO 300 I=1,NX
                             X1(I)     = XEXT(I)
                             VMOVE1(I) = VMOVE3(I)
 300                       CONTINUE
                           GOTO 330
 310                   CONTINUE
                           FEXT2 = FEXT3
                           DO 320 I=1,NX
                             X2(I)     = XEXT(I)
                             VMOVE2(I) = VMOVE3(I)
 320                       CONTINUE
 330                   CONTINUE
C
C.... KOHTPO�� �OCT��H�TO� TO�HOCT�.
C
                       EPSR = DABS(X2(NFIX1)-X1(NFIX1))
                       DF = DABS((FEXT1-FEXT2)/(X1(NFIX1)-X2(NFIX1)))
                       FN=    DABS (FEXT3/DF)
                       IF (EPSR.LE.EPSE .OR. FN.LE.0.1*EPSE) GOTO 360
 350                CONTINUE
C
C.... �A�AHHA� TO�HOCT� HE �OCT��H�TA.
C
                    INDE   =-1
                    ITER   = ITMAX
                    IPRTCL = 2
                    GOTO 362
 360                CONTINUE
C
C.... KOOP��HAT� TO�K� �KCTPEM�MA �TO�HEH� C �A�AHHO� TO�HOCT��.
C
                    IPRTCL = 1
 362                CONTINUE
#if defined(hib)
                    CALL WRTEXT (IPRTCL,IWRT,IPRE,ITER,IT,NCAN,NX,
     A                           NDIM,NEXT,IEXT,EPSE,EPSR,XEXT,
     B                           XABD,VMOVE3,DFUN)
#endif
 365                CONTINUE
C
C.... O�PA�OTKA TO�K� �KCTPEM�MA �O���OBATE�EM.
C
                    INDEX(4) = NEXT*IEXT
                    CALL OUT (ISTEP,INDEX,NX,NDIM,XEXT,VMOVE3,
     A                        DFUN,ISTOP)
                    IF (ISTOP .NE. 0) RETURN
 370            CONTINUE
 380        CONTINUE
 390      CONTINUE
 400  CONTINUE
C
C.... O�PA�OTKA �EPBO� TO�K� HA KP�BO�       (ISTEP=0);
C     �O��OTOBKA K O�EPE�HOM� �A�� �O KP�BO� (ISTEP>0).
C
          DMOVE1 = DMOVE2
          DO 410 I=1,NX
            STX1(I) =  STX2(I)
            STV1(I) =  STV2(I)
 410      CONTINUE
          RETURN
          END
