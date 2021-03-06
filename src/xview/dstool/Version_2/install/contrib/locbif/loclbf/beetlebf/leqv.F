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

      SUBROUTINE LEQV (A,B,NDIMM,NX,IER)
C     3.05.90�.
C     13.11.92
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA  LEQV  �PE�HA�HA�EHA ��� CT�KOBK� �PO�PAMM�            *
C*    BEETLE  C �PO�PAMMAM� PE�EH�� ��HEH�H�X C�CTEM                   *
C*                                                                     *
C*                     DECOMP  �  SOLVE .                              *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*    �P� ��MEHEH�� MAKC�MA��HO�O ��C�A  NDIM �EPEMEHH�X HEO�XO��MO    *
C*    ��MEH�T� PA�MEPHOCT� MACC�BOB: IPVT(*) WORK(*).                  *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2 (I-N)
      include 'beetlebf.dim'
C!!!  DIMENSION A(NDIM,NX), B(1), IPVT(25), WORK(25)
      DIMENSION A(NDIM,NX), B(1), IPVT(NDIM), WORK(NDIM)
C
      COMMON /LEQV1/ DETL, CONDL
      COMMON /CORR1/ DET,  COND, SGNDET
c******************* ��ࠢ�塞 ������� *********************
CCCCC COMMON /ALL2/  DETSTP
      COMMON /ALL4/  DETSTP
c******************* ��ࠢ�塞 ������� *********************
C
      COMMON /ADD1A/ IPR,  IPARC1, IPARC2
      COMMON /ADD2A/ ICRS, IPARC3

c      WRITE (6,*) ' LEQV:  DET=',DET,'  DETL=',DETL
C
C.... O�EH�M O��C�OB�EHHOCT� (COND) MATP��� A �
C     B���C��M O�PE�E��TE�� MATP��� (DET).
C
      CALL DECOMP (NDIM,NX,A,CONDL,IPVT,WORK)
C
      IF (IPR.LT.1 .AND. ICRS.EQ.0) GOTO 135
C
C.... B���C�EH�E O�PE�E��TE�� MATP��� A.
C
         DETL = IPVT(NX)
         DO 130 I=1,NX
            DETL = DETL*A(I,I)
 130     CONTINUE
         DET  = DETL
         COND = CONDL
         IF (DABS(DETL) .LE. DETSTP) GOTO 140
 135  CONTINUE
          CONDP1  =   CONDL + 1.D0
      IF (CONDP1 .EQ. CONDL)      GOTO 140
C
C.... PE��M ��HE�H�� C�CTEM�  A*X=B
C
         CALL SOLVE (NDIM,NX,A,B,IPVT)
         IER = 0
c      WRITE (6,*) ' LEQV:  DET=',DET,'  DETL=',DETL
         RETURN
 140  CONTINUE
      IER = -1
      RETURN
      END
