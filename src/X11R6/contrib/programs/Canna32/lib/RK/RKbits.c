/* Copyright 1994 NEC Corporation, Tokyo, Japan.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of NEC
 * Corporation not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission.  NEC Corporation makes no representations about the
 * suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * NEC CORPORATION DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN 
 * NO EVENT SHALL NEC CORPORATION BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
 * OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE. 
 */

#if !defined(lint) && !defined(__CODECENTER__)
static char rcsid[]="$Id: RKbits.c,v 2.6 1994/06/01 06:54:09 misao Exp $";
#endif
/* LINTLIBRARY */

/*
 * PackBits
 */
#define	BIT_UNIT	8

/*
  �ؽ��ե������ѥӥå�������

  �ؽ����ե��åȤξ���򡢤Ǥ�������������ӥåȤ�����Ȥ����ݻ����롣

    �����     1 2 3 4 5 6 7 8 9 ...    n
    �ӥå���   2 3 3 4 4 4 4 5 5     log(n) + 1

  RkPackBits �� unsigned ������ο��� dst_bits �� dst_offset(�ӥåȤ�
  �������)���褫���Ǽ���롣ü�����Ф����ϲ��̥ӥåȤ���Ȥ��롣

  ����
    dst_bits   -- �ӥå�����ؤΥݥ���
    dst_offset -- �ºݤ˥�����������Ȥ���ޤǤΥ��ե��å�(�ӥåȤǥ������)
    bit_size   -- �ӥå�����Σ��Ĥ����ǤΥӥå���
    src_ints   -- ��Ǽ���������ͤ�����
    count      -- ��Ǽ��������

  �����

 */

int
_RkPackBits(dst_bits, dst_offset, bit_size, src_ints, count)
unsigned char	*dst_bits;
int		dst_offset;
int		bit_size;
unsigned	*src_ints;
int		count;
{
  unsigned char	*dstB;
  unsigned		dstQ;
  unsigned		dstCount;
  unsigned		bitMask;
  
  dstB = dst_bits + dst_offset / BIT_UNIT;
  dstCount = (dst_offset % BIT_UNIT);

  /* ����ʤΤǡ�����դ��ʤ���ʬ�����뤳�Ȥ���� */
  dstQ  = *dstB & ((1 << dstCount) - 1);
  bitMask = (1 << bit_size) - 1;
  while (count-- > 0) {
    dstQ |= (*src_ints++ & bitMask) << dstCount;
    dstCount += bit_size;
    dst_offset += bit_size;
    while (dstCount >= BIT_UNIT) {
      *dstB++ = dstQ & ((1 << BIT_UNIT) - 1);
      dstQ >>= BIT_UNIT;
      dstCount -= BIT_UNIT;
    }
  }
  if (dstCount) {
    *dstB = (*dstB & ~((1 << dstCount) - 1)) | (dstQ & ((1 << dstCount) - 1));
  }
  return dst_offset;
}

/*
  UnpackBits

  RkUnpackBits �� dst_bits �� dst_offset(�ӥåȤǥ������)�˳�Ǽ�����
  ����ӥåȤ������ unsigned ������˼��Ф���offset ��ü�����Ф���
  ��ϲ��̥ӥåȤ���Ȥ��롣

  ����
    dst_ints   -- ���Ф������ͤ��Ǽ��������ؤΥݥ���
    src_bits   -- �ӥå�����ؤΥݥ���
    src_offset -- �ºݤ˳�Ǽ����Ȥ���ޤǤΥ��ե��å�(�ӥåȤǥ������)
    bit_size   -- �ӥå�����Σ��Ĥ����ǤΥӥå���
    count      -- ���Ф�������

  �����

 */

int
_RkUnpackBits(dst_ints, src_bits, src_offset, bit_size, count)
unsigned	*dst_ints;
unsigned char	*src_bits;
int		src_offset;
int		bit_size;
int		count;
{
  unsigned char	*srcB;
  unsigned		srcQ;
  unsigned		srcCount;
  unsigned		bitMask;
  
  srcB = src_bits + src_offset / BIT_UNIT;
  srcCount = BIT_UNIT - (src_offset % BIT_UNIT);
  srcQ  = *srcB++ >> (src_offset % BIT_UNIT);
  bitMask = (1 << bit_size) - 1;
  while (count-- > 0) {
    while (srcCount < bit_size) {
      srcQ |= (*srcB++ << srcCount);
      srcCount += BIT_UNIT;
    }
    *dst_ints++ = srcQ & bitMask;
    srcQ >>= bit_size;
    srcCount -= bit_size;
    src_offset += bit_size;
  }
  return src_offset;
}

/*
  CopyBits

  RkCopyBits �� src_bits �� src_offset �˳�Ǽ����Ƥ���ӥå������ 
  count �Ĥ��� dst_bits �� dst_offset�˰�ư�����롣

  ����
    dst_bits   -- ��ư��ӥå�����ؤΥݥ���
    dst_offset -- �ºݤ˳�Ǽ����Ȥ���ޤǤΥ��ե��å�(�ӥåȤǥ������)
    bit_size   -- �ӥå�����Σ��Ĥ����ǤΥӥå���
    src_bits   -- ��ư���ӥå�����ؤΥݥ���
    src_offset -- ���Ф��Ȥ���ޤǤΥ��ե��å�(�ӥåȤǥ������)
    count      -- ��ư��������

  �����

 */

int
_RkCopyBits(dst_bits, dst_offset, bit_size, src_bits, src_offset, count)
unsigned char	*dst_bits;
int		dst_offset;
int		bit_size;
unsigned char	*src_bits;
int		src_offset;
int		count;
{
  unsigned char	*dstB;
  unsigned		dstQ;
  unsigned		dstCount;
  unsigned char	*srcB;
  unsigned		srcQ;
  unsigned		srcCount;
  unsigned		bitMask;
  unsigned		bits;
  
  dstB = dst_bits + dst_offset / BIT_UNIT;
  dstCount = (dst_offset % BIT_UNIT);
  dstQ  = *dstB & ((1 << dstCount) - 1);
  srcB = src_bits + src_offset / BIT_UNIT;
  srcCount = BIT_UNIT - (src_offset % BIT_UNIT);
  srcQ  = *srcB++ >> (src_offset % BIT_UNIT);
  bitMask = (1 << bit_size) - 1;
  while (count-- > 0) {
    /* unpack */
    while (srcCount < bit_size) {
      srcQ |= (*srcB++ << srcCount);
      srcCount += BIT_UNIT;
    }
    bits = srcQ & bitMask;
    srcQ >>= bit_size;
    srcCount -= bit_size;
    src_offset += bit_size;
    /* pack */
    dstQ |= bits << dstCount;
    dstCount += bit_size;
    dst_offset += bit_size;
    while (dstCount >= BIT_UNIT) {
      *dstB++ = dstQ & ((1 << BIT_UNIT) - 1);
      dstQ >>= BIT_UNIT;
      dstCount -= BIT_UNIT;
    }
  }
  if (dstCount) {
    *dstB = (*dstB & ~((1 << dstCount) - 1)) | (dstQ & ((1 << dstCount) - 1));
  }
  return dst_offset;
}

/*
  _RkSetBitNum

  _RkSetBitNum �� bit ����� offset ���֤��� n ���ܤ��ͤȤ��� val ���
  Ǽ����ؿ��Ǥ��롣

  ����
    dst_bits   -- �ӥå�����ؤΥݥ���
    dst_offset -- �ºݤ˳�Ǽ����Ȥ���ޤǤΥ��ե��å�(�ӥåȤǥ������)
    bit_size   -- �ӥå�����Σ��Ĥ����ǤΥӥå���
    n          -- ��Ƭ���鲿���ܤ����Ǥ���Ϳ���롣
    val        -- ��Ǽ�����ͤ�Ϳ���롣

  �����

 */

int
_RkSetBitNum(dst_bits, dst_offset, bit_size, n, val)
unsigned char	*dst_bits;
int		dst_offset;
int		bit_size, n, val;
{
  unsigned char	*dstB;
  unsigned dstQ, dstCount, bitMask;

  dst_offset += bit_size * n;

  dstB = dst_bits + dst_offset / BIT_UNIT;
  dstCount = (dst_offset % BIT_UNIT);

  /* ����ʤΤǡ�����դ��ʤ���ʬ�����뤳�Ȥ���� */
  dstQ  = *dstB & ((1 << dstCount) - 1);
  bitMask = (1 << bit_size) - 1;

  dstQ |= (val & bitMask) << dstCount;
  dstCount += bit_size;
  dst_offset += bit_size;
  while (dstCount >= BIT_UNIT) {
    *dstB++ = dstQ & ((1 << BIT_UNIT) - 1);
    dstQ >>= BIT_UNIT;
    dstCount -= BIT_UNIT;
  }
  if (dstCount) {
    *dstB = (*dstB & ~((1 << dstCount) - 1)) | (dstQ & ((1 << dstCount) - 1));
  }
  return dst_offset;
}

int
_RkCalcFqSize(n)
int	n;
{
  return n*(_RkCalcLog2(n) + 1);
}

#ifdef __BITS_DEBUG__
#include <stdio.h>
_RkPrintPackedBits(bits, offset, bit_size, count)
unsigned char	*bits;
int		offset;
int		bit_size;
int		count;
{
    fprintf(stderr, "%d <", count);
    while ( count-- > 0 ) {
	unsigned w;

        offset = _RkUnpackBits(&w, bits, offset, bit_size, 1);
        fprintf(stderr, " %d", w/2);
    };
    fprintf(stderr, ">\n");
}

int 
_RkCalcLog2(n)
     int n;
{
  int	lg2;
  
  n--;
  for (lg2 = 0; n > 0; lg2++)
    n >>= 1;
  return(lg2);
}

main()
{
  int		 offset;
  int		 bit_size;
  int		 size;
  unsigned char bits[1024*8];
  unsigned char Bits[1024*8];
  int	c, i;
  int		ec;
  int	o;
  
  /* create test run */
  for ( size = 1; size <= 32; size++ ) {
    bit_size = _RkCalcLog2(size) + 1;
    printf("#%4d/%2d\t", size, bit_size);
    /* pack 'em all */
    o = 0;
    for ( i = 0; i < size; i++ )
      o = _RkPackBits(Bits, o, bit_size, &i, 1);
    printf("PK ");
    for ( i = 0; i < (bit_size*size+7)/8; i++ )
      printf(" %02x", Bits[i]);
    printf("\n");
    
    
    for ( offset = 0; offset < 16; offset++ ) {
      /* copybits */
      o = _RkCopyBits(bits, offset, bit_size, Bits, 0, size);
      printf("%d ", offset);
      for ( i = 0; i < (o + 7)/8; i++ )
	printf(" %02x", bits[i]);
      printf("\n");
      
      /* unpack 'em again */
      ec = 0;
      o = offset;
      for ( i = 0; i < size; i++ ) {
	unsigned w;
	  
	o = _RkUnpackBits(&w, bits, o, bit_size, 1);
	if ( w != i )
	  ec++;
      };
      if ( ec )
	printf(" %d", offset);
      else
	printf(".");
    };
    printf("\n");
  };
}
#endif
