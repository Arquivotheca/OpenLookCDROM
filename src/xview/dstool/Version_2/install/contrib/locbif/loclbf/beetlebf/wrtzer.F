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

      SUBROUTINE WRTZER (IPRTCL,INDF,IWRT,ISTEP,IPRZ,IT,IDIV,NCAN,NX,
     A                   NDIM,ITER,NUF,IFUNS,EPSR,EPSF,X1,X2,FX,DFUN)
C     3.05.90�.
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA  WRTZER  �PE�HA�HA�EHA ��� B�BO�A �POTOKO�A            *
C*    �TO�HEH�� KOOP��HAT H��E� �A�AHH�X ��HK���.                      *
C*                                                                     *
C*        � H A � E H � � �APAMETPA  IPRTCL:                           *
C*                                                                     *
C*         1 -B�BO� KOOP��HAT H��� �A�AHHO� ��HK���;                   *
C*                                                                     *
C*                                                                     *
C*         2 -B�BO� COO��EH��: �PEKPA�EHO �TO�HEH�E KOOP��HAT H���     *
C*            �A�AHHO� ��HK���;                                        *
C*                                                                     *
C*         3 -B�BO� B���C�EHH�X �HA�EH�� �A�AHH�X ��HK��� B TEK��E�    *
C*            TO�KE;                                                   *
C*                                                                     *
C*                                                                     *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2(I-N)
      DIMENSION X1(1), X2(1), DFUN(NDIM,NX), IFUNS(1)
C
#if defined(hib)
      IF (IPRZ .LT. 0) RETURN
          GOTO (100,120,130), IPRTCL
 100  CONTINUE
C
C.... IPRTCL = 1
C     COO��EH�E: �TO�HEH� KOOP��HAT� H���.
C
          WRITE (NCAN,2000)
          WRITE (NCAN,4020)
          WRITE (NCAN,2010) INDF
          IF (ITER .EQ. 10) WRITE (NCAN,2020)
          WRITE (NCAN,2030) EPSR
          IF (IPRZ .LT. 1) GOTO 110
              WRITE (NCAN,2040) EPSF
              WRITE (NCAN,4020)
              WRITE (NCAN,2050) FX
              WRITE (NCAN,2070) ITER
 110      CONTINUE
          WRITE (NCAN,4020)
          WRITE (NCAN,2080)
          WRITE (NCAN,4020)
          WRITE (NCAN,2090) (X1(I),I=1,NX)
          WRITE (NCAN,4020)
          WRITE (NCAN,2000)
          WRITE (NCAN,3000)
          RETURN
 120  CONTINUE
C
C.... IPRTCL = 2
C     COO��EH�E: �PEKPA�EHO �TO�HEH�E KOOP��HAT� H���.
C
          WRITE (NCAN,2000)
          WRITE (NCAN,4020)
          WRITE (NCAN,2010) INDF
          WRITE (NCAN,2050) FX
          WRITE (NCAN,4020)
          WRITE (NCAN,2030) EPSR
          WRITE (NCAN,2040) EPSF
          WRITE (NCAN,4020)
          WRITE (NCAN,2070) ITER
          WRITE (NCAN,4020)
          WRITE (NCAN,2080)
          WRITE (NCAN,4020)
          WRITE (NCAN,2090) (X1(I),I=1,NX)
          WRITE (NCAN,4020)
          WRITE (NCAN,3080)
          IF (IWRT .EQ. 0) WRITE (NCAN,3090) (X2(I),I=1,NX)
          IDIV = 1
          IPR  = 2
          IF (IWRT .NE. 0) CALL  WRTOUT (IWRT,ISTEP,IPR,IT,IDIV,NCAN,
     A                                   NX,NDIM,X2,DFUN)
          WRITE (NCAN,4020)
          WRITE (NCAN,2000)
          WRITE (NCAN,3000)
          RETURN
 130  CONTINUE
C
C.... IPRTCL = 3
C     B�BO� B���C�EHH�X �HA�EH�� �A�AHH�X ��HK���.
C
          IF (IPRZ .LT. 1)      RETURN
              DO 140 I=1,NUF
                 INDF = IABS(IFUNS(I))
                 X2(I)= X1(INDF)
 140          CONTINUE
              WRITE (NCAN,4020)
              WRITE (NCAN,4000) (IFUNS(I), X2(I),I=1,NUF)
              WRITE (NCAN,3000)
#endif
              RETURN
C
#if defined(hib)
 2000 FORMAT
     A(1X,120('-'))
C
 2010 FORMAT
     A('   �PO��EHO H��EBOE �HA�EH�E ',I4,'  -��HK���.'/
     B '   -----------------------------------------')
C
 2020 FORMAT
     A('   ( �A�AHHA� TO�HOCT� HE �OCT��H�TA �A 10 �TEPA���.)')
C
 2030 FORMAT
     A('   �OCT��H�TA TO�HOCT�:                      ',G14.5)
 2040 FORMAT
     A('   �A�AHHA� TO�HOCT�:                        ',G14.5)
C
 2050 FORMAT
     A('   �HA�EH�E ��HK����:                        ',G14.5)
C
 2070 FORMAT
     A('   �PO�E�AHO �A�OB:                          ',I5)
C
 2080 FORMAT
     A('   KOOP��HAT� TO�K� HA KP�BO�:               ')
C
 2090 FORMAT
     A(10(2X,7(G14.5,1X)/))
C
 3000 FORMAT
     A(1X,120('.'))
C
 3080 FORMAT
     A('   HE HA��EHO O�EPE�HOE �P�����EH�E K H���:       ')
C
 3090 FORMAT
     A('   HE B���C�EHO �HA�EH�E ��HK��� B TO�KE HA KP�BO� C ',
     B 'KOOP��HATAM�:'/10(2X,7(G14.5,1X)/))
 4000 FORMAT
     A(10(2X,5(' G(',I3,')=',G14.5,1X)))
 4020 FORMAT
     A(1X)
#endif
          END
