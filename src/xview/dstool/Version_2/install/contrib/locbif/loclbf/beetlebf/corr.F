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

      SUBROUTINE CORR (NX,NDIMM,NFIX1,NFIX,VMOVE,DMOVE,DFUN)
C     3.05.90�.
C     13.11.92
C     15.06.94
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    Subroutine CORR serves for:
C*       1) correction of 
C*       1) KOPPEK��� KACATE��HO�O BEKTOPA;                            *
C*       2) CMEH� MATP���, HEO�XO��MO� ��� KOHTPO�� TO�EK              *
C*          CAMO�EPECE�EH��;                                           *
C*    �PO�PAMMA CORR B���BAETC� TO��KO TO��A, KO��A �PO�CXO��T �AMEHA  *
C*    ��KC�POBAHHO� �EPEMEHHO�.                                        *
C*                                                                     *
C*    C��COK �OPMA��H�X � A P A M E T P O B:                           *
C*                                                                     *
C*    NX   -PA�MEPHOCT� �POCTPAHCTBA �EPEMEHH�X;                       *
C*    NDIM -�A�B�EHHA� CTPO�HA� PA�MEPHOCT� MATP��� DFUN;              *
C*    NFIX1-CTAPOE �HA�EH�E ��KC�POBAHHO� �EPEMEHHO�;                  *
C*    NFIX -HOBOE �HA�EH�E ��KC�POBAHHO� �EPEMEHHO�;                   *
C*    VMOVE-HA BXO�E: KOOP��HAT� KACATE��HO�O BEKTOPA C VMOVE(NFIX1)>0;*
C*          HA B�XO�E:KOOP��HAT� KACATE��HO�O BEKTOPA C VMOVE(NFIX)>0; *
C*    DMOVE-�A� B�O�� KP�BO�: HA BXO�E � HA B�XO�E �� �PO�PAMM�,       *
C*          BE����HA  DMOVE*VMOVE(NFIX1) COXPAH�ET �HAK;               *
C*    DFUN -MATP��A �ACTH�X �PO��BO�H�X.                               *
C*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*
C*                                                                     *
C*    B���BAEM�E �PO�PAMM�: DECOMP.                                    *
C*                                                                     *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*   �P� ��MEHEH�� MAKC�MA��HO�O ��C�A �EPEMEHH�X  NDIM  HEO�XO��MO    *
C*   ��MEH�T� PA�MEPHOCT� MACC�BOB: A(*,*) IPVT(*) WORK(*)             *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2 (I-N)
      include 'beetlebf.dim'
      DIMENSION VMOVE(NX), DFUN(NDIM,NX), IPVT(NDIM), WORK(NDIM)
      COMMON /CORR1/ DET, COND,SGNDET
      COMMON /LEQV1/ DETL, CONDL
C
      COMMON /WORKBE/ ARR1(NDIM,NDIM), A(NDIM,NDIM)
C
C.... KOPPEK��� OP�EHTA��� KACATE��HO�O BEKTOPA.
C
c      WRITE (6,*) ' CORR:  DET=',DET,'  DETL=',DETL
      IF (VMOVE(NFIX) .GT. 0.D0) GOTO 210
          DO 200 I=1,NX
             VMOVE(I) = -1.D0*VMOVE(I)
 200      CONTINUE
          DMOVE = -1.D0*DMOVE
 210  CONTINUE
C
C.... KOPPEK��� MATP��� �ACTH�X �PO��BO�H�X.
C
      DFUN(NX,NFIX1) = 0.0D0
      DFUN(NX,NFIX)  = 1.0D0
C
C.... �O��OTOBKA PA�O�E� MATP��� A ��� B���C�EH�� O�PE�E��TE��.
C
      DO 230 I=1,NX
         DO 220 J=1,NX
            A(I,J) = DFUN(I,J)
 220     CONTINUE
 230  CONTINUE
C
C.... B���C�EH�E O�PE�E��TE�� (DET), COOTBETCTB���E�O HOBO�
C     ��KC�POBAHHO� �EPEMEHHO�  NFIX.
C
      CALL DECOMP (NDIM,NX,A,COND,IPVT,WORK)
      DET = IPVT(NX)
      DO 310 I=1,NX
         DET = DET*A(I,I)
 310  CONTINUE
c      WRITE (6,*) ' CORR:  DET=',DET,'  DETL=',DETL
      RETURN
      END
