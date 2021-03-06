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

      SUBROUTINE WRTEXT (IPRTCL,IWRT,IPRE,ITER,ITNWT,NCAN,NX,NDIM,NEXT,
     A                   IEXT,EPSE,EPSR,XEXT,XABD,VMOVE,DFUN)
C     3.05.90�.
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA  WRTEXT  �PE�HA�HA�EHA ��� �E�AT� �POTOKO�A �TO�HEH��  *
C*    TO�EK HA KP�BO�, B KOTOP�X �A�AHH�E �EPEMEHH�E �OCT��A�T CBO�X   *
C*    �KCTPEMA��H�X �HA�EH��.                                          *
C*                                                                     *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2(I-N)
      DIMENSION XEXT(1), XABD(1), VMOVE(1), DFUN(NDIM,NX)
C
#if defined(hib)
      IF (IPRE .LT. 0) RETURN
          WRITE (NCAN,2000)
          WRITE (NCAN,2010)
          WRITE (NCAN,2020)      NEXT
          IF (IPRTCL .EQ. 2)     WRITE (NCAN,2022)
          IF (IEXT .GT. 0.D0)    WRITE (NCAN,2030) NEXT, XEXT(NEXT)
          IF (IEXT .LT. 0.D0)    WRITE (NCAN,2035) NEXT, XEXT(NEXT)
          WRITE (NCAN,2010)
          WRITE (NCAN,2040)      EPSR
          IF (IPRTCL .EQ. 2)     WRITE (NCAN,2060) EPSE
          IF (IPRTCL .EQ. 3)     WRITE (NCAN,2080)
          IF (IPRE   .LT. 1)     GOTO 200
C
C.... CTAH�PTH�E COO��EH��: 1) HE CO���C� H��TOHOBCK�E �TEPA���;
C                           2) HE HA��EH KACATE��H�� BEKTOP;
C
              IDIV = 1
              IPR  = 2
              IF (IPRTCL .EQ. 3) CALL WRTOUT (IWRT,ISTEP,IPR,ITNWT,IDIV,
     A                                        NCAN,NX,NDIM,XABD,DFUN)
              IF (IPRTCL .EQ. 3) WRITE (NCAN,2070)
              WRITE (NCAN,2010)
              WRITE (NCAN,3000)  ITER
              WRITE (NCAN,2010)
              WRITE (NCAN,3010)
              WRITE (NCAN,2010)
              WRITE (NCAN,3020)  (VMOVE(I),I=1,NX)
 200  CONTINUE
              WRITE (NCAN,2010)
              WRITE (NCAN,3030)
              WRITE (NCAN,2010)
              WRITE (NCAN,3020)  (XEXT(I),I=1,NX)
              WRITE (NCAN,2010)
              WRITE (NCAN,2000)
              WRITE (NCAN,3040)
#endif
              RETURN
C
C.... �OPMAT� �POTOKO�A.
C
#if defined(hib)
 2000 FORMAT
     A(1X,120('-'))
C
 2010 FORMAT
     A(1X)
C
 2020 FORMAT
     A('   �PO��EHO �KCTPEMA��HOE �HA�EH�E ',I5,'  -�EPEMEHHO�.    '/
     B '   ---------------------------------------------------     ')
C
 2022 FORMAT
     A('   TPE��EMA� TO�HOCT� HE �OCT��H�TA.')
C
 2030 FORMAT
     A('   MAKC�MA��HOE �HA�EH�E ',I4,' �EPEMEHHO�:          ',G14.5)
C
 2035 FORMAT
     A('   M�H�MA��HOE �HA�EH�E  ',I4,' �EPEMEHHO�:          ',G14.5)
C
 2040 FORMAT
     A('   �OCT��H�TA TO�HOCT�:                              ',G14.5)
C
 2060 FORMAT
     A('   �A�AHHA�   TO�HOCT�:                              ',G14.5)
C
 2070 FORMAT
     A(1X,120('-'))
C
 2080 FORMAT
     A('   HE HA��EHO O�EPE�HOE �P�����EH�E  K TO�KE �KCTPEM�MA.')
C
 3000 FORMAT
     A('   �PO�E�AHO �A�OB �TO�HEH��:                        ',I6)
C
 3010 FORMAT
     A('   KOOP��HAT� KACATE��HO�O BEKTOPA:                  ')
C
 3020 FORMAT
     A(10(4X,7(G14.5,2X)/))
C
 3030 FORMAT
     A('   KOOP��HAT� TO�K� �KCTPEM�MA:                      ')
C
 3040 FORMAT
     A(1X,120('.'))
C
#endif
      END
