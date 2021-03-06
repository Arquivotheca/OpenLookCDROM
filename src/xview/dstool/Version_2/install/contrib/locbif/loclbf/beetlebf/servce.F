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

      SUBROUTINE SERVCE (ISTEP,NX,NDIMM,NG,NCAN,ICRS,NUF,IFUNS,NUE,
     A		 IVARS,IPRNT,XCRV,EPS,FUN,DFDX,OUT,DFUN,ISTOP)
C     3.05.90�.
C     13.11.92
C     15.06.94
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA  SERVCE  �PE�HA�HA�EHA ��� PEA���A��� �O�O�H�TE��H�X   *
C*    BO�MO�HOCTE� �PO�E��P� �B��EH�� �O KP�BO�:                       *
C*                                                                     *
C*       1. O�PA�OTKA BHOB� HA��EHHO� TO�K� �O���OBATE�EM.             *
C*       2. �TO�HEH�E TO�EK CAMO�EPECE�EH�� KP�BO�.                    *
C*       3. �TO�HEH�E H��E� �A�AHH�X ��HK���.                          *
C*       4. �TO�HEH�E �KCTPEM�MOB �A�AHH�X �EPEMEHH�X.                 *
C*                                                                     *
C*    �PO�PAMMA  SERVCE  B���BAETC� �PO�PAMMO�  BEETLE  CPA�� �OC�E    *
C*    �TO�HEH�� O�EPE�HO� TO�K� HA KP�BO�.                             *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*    �P� ��MEHEH�� MAKC�MA��HO�O ��C�A �EPEMEHH�X- NDIM  HEO�XO��MO   *
C*    ��MEH�T� PA�MEPHOCT� MACC�BA VMOVE (*).                          *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2 (I-N)
      include 'beetlebf.dim'
      EXTERNAL FUN, DFDX, OUT
      DIMENSION XCRV(1), DFUN(NDIM,NX), IFUNS(1), IVARS(1), IPRNT(1),
     A          INDEX(NPTYP)
      COMMON /WRTO7/ VMOVE(NDIM),VMOVN(NDIM)
      COMMON /LEQV1/ DETL, CONDL
      COMMON /CORR1/ DET,  COND, SGNDET
      COMMON /OUT8/  INDOUT
C
      ISTOP  = 0
      INDOUT = 0
C
C.... �A�OMH�M TEK��EE � HOBOE �HA�EH�� O�PE�E��TE�� �
C     O��C�OB�EHHOCT� M�HOPOB, H��H�X ��� KOHTPO�� TO�EK
C     CAMO�EPECE�EH��.
C
      RDETL = DETL
      RCONDL= CONDL
      RDET  = DET
      RCOND = COND
C
C.... �O�O�H�TE��HA� O�PA�OTKA TEK��E� TO�K� HA KP�BO�.
C
      IF (NUF .NE. 0) GOTO 200
          INDEX(1) = 1
          CALL OUT (ISTEP,INDEX,NX,NDIM,XCRV,VMOVE,DFUN,ISTOP)
          IF (ISTOP .NE. 0) RETURN
 200  CONTINUE
C
C.... �TO�HEH�E KOOP��HAT H��EB�X �HA�EH�� �A�AHH�X ��HK���.
C
      IPRZ = IPRNT(3)
      CALL ZERO (ISTEP,NX,NDIM,NG,NCAN,NUF,IFUNS,IPRZ,XCRV,EPS,
     A           FUN,DFDX,OUT,DFUN,ISTOP)
      IF (ISTOP .NE. 0) RETURN
C
C.... �TO�HEH�E TO�EK CAMO�EPECE�EH��.
C
      DETL = RDETL
      CONDL= RCONDL
      DET  = RDET
      COND = RCOND
C
      IPRCRS = IPRNT(2)
      CALL SLFCRS (ISTEP,NX,NDIM,NG,NCAN,ICRS,IPRCRS,XCRV,EPS,
     A             FUN,DFDX,OUT,DFUN,ISTOP)
      IF (ISTOP .NE. 0) RETURN
C
C.... �TO�HEH�E TO�EK �KCTPEM�MA �A�AHH�X KOOP��HAT HA KP�BO�.
C
      IPRE = IPRNT(4)
      CALL EXTR (ISTEP,NX,NDIM,NG,NCAN,NUE,IVARS,IPRE,XCRV,EPS,
     A           FUN,DFDX,OUT,DFUN,ISTOP)
      IF (ISTOP .NE. 0) RETURN
C
C.... �OPMA��H�� B��OB �PO�PAMM�  OUT -�O�HOCT�� �AKOH�EHA O�PA�OTKA
C     BCEX TO�EK KP�BO� HA O�EPE�HOM �A�E.
C
      INDOUT = 1
      CALL OUT (ISTEP,INDEX,NX,NDIM,XCRV,VMOVE,DFUN,ISTOP)
      RETURN
      END
