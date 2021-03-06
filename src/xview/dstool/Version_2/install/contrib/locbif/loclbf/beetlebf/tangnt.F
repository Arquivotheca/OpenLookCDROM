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

        SUBROUTINE TANGNT(NX,NDIMM,NFIX,XTNG,FTNG,VTNG,FUN,DFDX,DFUN,
     A                    IER,ISTOP)
C       3.05.90�.
C       31.07.1991
C       13.11.92
C       15.06.94
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA  TANGNT  �PE�HA�HA�EHA ��� HAXO��EH�� BEKTOPA          *
C*    E��H��HO� ���H� - VTNG, KACATE��HO�O K KP�BO� : FUN(X)=0         *
C*    B TO�KE XTNG.                                                    *
C*    �PE��O�A�AETC�, �TO B KA�ECTBE �APAMETPA HA KP�BO� B��PAHA       *
C*    NFIX-KOM�OHEHTA BEKTOPA X: X(NFIX).                              *
C*    KACATE��H�� BEKTOP OP�EHT�P�ETC� TAK, �TO�� �B��EH�E �O KACATE��-*
C*    HOM� BEKTOP� COOTBETCTBOBA�O BO�PACTAH�� �APAMETPA, T.E.         *
C*    BCE��A B��O�H�ETC� HEPABEHCTBO: VTNG(NFIX) GT. 0.D0              *
C*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*
C*                                                                     *
C*    �APAMETP�:                                                       *
C*    =========                                                        *
C*    NX   - ��C�O �EPEMEHH�X;                                         *
C*    NDIM - �A�B�EHHA� CTPO�HA� PA�MEPHOCT� MATP��� DFUN;             *
C*    NFIX - HOMEP ��KC�POBAHHO� �EPEMEHHO�;                           *
C*    XTNG - MACC�B KOOP��HAT TO�K� HA KP�BO�, B KOTOPO� B���C��ETC�   *
C*           KACATE��H�� BEKTOP.                                       *
C*    FTNG - ARRAY FOR FUNCTION VALUES                                 *
C*    VTNG - MACC�B KOOP��HAT KACATE��HO�O BEKTOPA.                    *
C*    FUN  - �M� �PO�PAMM� B���C�EH�� �APB�X �ACTE� - F(X).            *
C*    DFDX - �M� �PO�PAMM� B���C�EH�� MATP��� �ACTH�X �PO��BO�H�X.     *
C*    DFUN - �B�MEPH�� MACC�B PA�MEPHOCT�  NDIM*NX, CO�EP�A���         *
C*           MATP��� �EPB�X �ACTH�X �PO��BO�H�X;                       *
C*    IER  - �APAMETP O���K�:                                          *
C*              0 - KACATE��H�� BEKTOP �C�E�HO HA��EH;                 *
C*             -1 - ��HE�HA� C�CTEMA �PABHEH��, O�PE�E����A�           *
C*                  KACATE��H�� BEKTOP - B�PO��EHA;                    *
C*             -2 - HE B���C�EH� �HA�EH�� ��HK��� B TO�KE  XTNG;       *
C*             -3 - HE B���C�EH� �PO��BO�H�E B TO�KE       XTNG;       *
C* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . *
C*                                                                     *
C*    B���BAEM�E �PO�PAMM�: DERN, FUN, JACOB.                          *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*    �P� HEO�XO��MOCT� ��MEHEH�� MAKC�MA��HO�O ��C�A NDIM             *
C*    �EPEMEHH�X, C�E��ET ��MEH�T� PA�MEPHOCT� MACC�BA A(NDIM,NDIM),   *
C*                                                   ARR1(NDIM,NDIM)   *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2 (I-N)
      include 'beetlebf.dim'
      EXTERNAL FUN, DFDX
      DIMENSION XTNG(1), FTNG(1), VTNG(1), DFUN(NDIM,NX)
C
      COMMON /WORKBE/ ARR1(NDIM,NDIM), A(NDIM,NDIM)
C
C.... B���C��EM �HA�EH�� ��HK��� F(X) B TO�KE XTNG;
C     B���C�EHH�E �HA�EH�� PA�ME�A�TC� B MACC�BE  VTNG.
C
      ISTOP = 0
      IER   = 0
      CALL FUN (XTNG,FTNG,IER,ISTOP)
      IF (ISTOP .NE. 0) RETURN
      IF (IER   .EQ. 0) GOTO 102
          IER = -2
          RETURN
 102  CONTINUE
C
C.... B���C��EM MATP��� �ACTH�X �PO��BO�H�X.
C
      DO 200 J=1,NX
         DFUN(NX,J) = 0.D0
 200  CONTINUE
      DFUN(NX,NFIX) = 1.D0
      IER = 0
      CALL JACOB (NX,NDIM,XTNG,FUN,DFDX,DFUN,FTNG,IER,ISTOP)
      IF (ISTOP .NE. 0) RETURN
      IF (IER   .EQ. 0) GOTO 215
          IER =-3
          RETURN
 215  CONTINUE
C
C.... �OPM�P�EM BEKTOP �PAB�X �ACTE� (VTNG) � MATP��� (A).
C
      DO 255 I=1,NX
         VTNG(I) = 0.0D0
         DO 250 J = 1,NX
            A(I,J) = DFUN(I,J)
 250     CONTINUE
 255  CONTINUE
      VTNG(NX)  = 1.D0
C
C.... PE�AEM C�CTEM� ��HE�H�X �PABHEH��  A*Y=VTNG;
C
      IER = 0
      CALL LEQV (A,VTNG,NDIM,NX,IER)
      IF (IER .EQ. 0) GOTO 260
C
C.... MATP��A  DFUN - B�PO��EHA.
C
          IER = -1
          RETURN
 260  CONTINUE
C
C.... HOPM�P�EM ���H� KACATE��HO�O BEKTOPA  VTNG  HA E��H���.
C
      DLEN = 0.D0
      DO 360 I = 1,NX
        ELEM = VTNG(I)
        DLEN = DLEN + ELEM*ELEM
 360  CONTINUE
      DLEN = DSQRT(DLEN)
      DO 380 I = 1,NX
        VTNG(I) = VTNG(I)/DLEN
 380  CONTINUE
      IER = 0
      RETURN
      END
