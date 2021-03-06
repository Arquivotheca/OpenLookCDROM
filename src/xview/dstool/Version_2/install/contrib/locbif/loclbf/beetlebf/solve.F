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

      SUBROUTINE SOLVE (NDIM,N,A,B,IPVT)
C     3.05.90�.
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2 (I-N)
      DIMENSION A(NDIM,N), B(N), IPVT(N)
C
C     PE�EH�E ��HE�HO� C�CTEM� A*X=B
C     �O��PO�PAMM� HE C�E��ET �C�O���OBAT�,
C     EC�� DECOMP O�HAP����A B�PO��EHHOCT�.
C
C     (�PO�PAMMA O��CAHA B KH��E:
C
C           ��.�OPCA�T, M.MA��KOM, K.MO��EP
C           "MA��HH�E METO�� MATEMAT��ECK�X B���C�EH��"
C            M.:M�P,1980)
C
C     BXO�HA� �H�OPMA���..
C
C       NDIM=�A�B�EHHA� CTPO�HA� PA�MEPHOCT� MAC-
C       C�BA, CO�EP�A�E�O A.
C
C       N=�OP��OK MATP���.
C
C       A=�AKTOP��OBAHHA� MATP��A, �O���EHHA� ��
C       DECOMP.
C
C       B=BEKTOP �PAB�X �ACTE�.
C
C       IPVT=BEKTOP BE����X ��EMEHTOB, �O���EHH��
C       �� DECOMP.
C
C     B�XO�HA� �H�OPMA���..
C
C       B=BEKTOP PE�EH��.
C
C
C     �P�MO� XO�.
C
      IF (N .EQ. 1) GOTO 50
      NM1 = N - 1
      DO 20 K=1,NM1
         KP1 = K + 1
         M   = IPVT(K)
         T   = B(M)
         B(M)= B(K)
         B(K)= T
         DO 10 I=KP1,N
            B(I) = B(I) + A(I,K)*T
 10      CONTINUE
 20   CONTINUE
C
C     O�PATHA� �O�CTAHOBKA
C
      DO 40 KB=1,NM1
         KM1 = N - KB
         K   = KM1 + 1
         B(K)=B(K)/A(K,K)
         T   = -B(K)
         DO 30 I=1,KM1
            B(I) = B(I) + A(I,K)*T
 30      CONTINUE
 40   CONTINUE
 50   B(1) = B(1)/A(1,1)
      RETURN
      END
