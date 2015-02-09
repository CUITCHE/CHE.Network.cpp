#ifndef HCodeConversion_H__
#define HCodeConversion_H__

/********************************************************************
	Copyright (C) 2014 CUIT��CHE.
	All rights reserved.
	Contact: 	CHE (hejun_753@163.com)
	created:	2014/10/15 12:37
	author:		CHE

	purpose:	Unicode����ת����������utf-8��utf-16��UCS2����utf-32��UCS4��֮��ı���ת��
	*********************************************************************/
#include "hglobal.h"
NAMESPACE_BEGIN_CHE
class HCodeConversion
{
public:
	HCodeConversion() = delete;
	~HCodeConversion() = delete;

	//************************************
	// Method:    UCS4_To_UTF8
	// FullName:  che::HCodeConversion::UCS4_To_UTF8
	// Access:    public static 
	// Returns:   int 0����Ч��UCS4���� 1-6��UTF8�������Ч����
	// Qualifier: ��UCS4����ת����UTF8����
	// Parameter: DWORD dwUCS4	��ת��ucs4����
	// Parameter: byte * pbUTF8	�洢utf8���룬��ΪNULL���ɻ�ȡ������Ϣ(�ֽ���)
	//************************************
	static int UCS4_To_UTF8(DWORD dwUCS4, byte *pbUTF8){
		static const byte	abPrefix[] = { 0, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
		static const DWORD adwCodeUp[] = {
			0x80,			// U+00000000 �� U+0000007F
			0x800,			// U+00000080 �� U+000007FF
			0x10000,		// U+00000800 �� U+0000FFFF
			0x200000,		// U+00010000 �� U+001FFFFF
			0x4000000,		// U+00200000 �� U+03FFFFFF
			0x80000000		// U+04000000 �� U+7FFFFFFF
		};

		int	i, iLen;

		// ����UCS4���뷶Χȷ����Ӧ��UTF-8�����ֽ���
		iLen = sizeof(adwCodeUp) / sizeof(DWORD);
		for (i = 0; i < iLen; i++){
			if (dwUCS4 < adwCodeUp[i]){
				break;
			}
		}

		if (i == iLen)return 0;	// ��Ч��UCS4����

		iLen = i + 1;	// UTF-8�����ֽ���
		if (pbUTF8 != NULL){	// ת��ΪUTF-8����
			for (; i > 0; i--){
				pbUTF8[i] = static_cast<byte>((dwUCS4 & 0x3F) | 0x80);
				dwUCS4 >>= 6;
			}

			pbUTF8[0] = static_cast<byte>(dwUCS4 | abPrefix[iLen - 1]);
		}

		return iLen;
	}

	//************************************
	// Method:    UTF8_To_UCS4
	// FullName:  che::HCodeConversion::UTF8_To_UCS4
	// Access:    public static 
	// Returns:   int	0�������������Ч��UTF8����	1-6��UTF8�������Ч����
	// Qualifier: ��UTF8����ת����UCS4����
	// Parameter: const byte * pbUTF8	Ҫת����UTF8����
	// Parameter: DWORD & dwUCS4		�洢ת�����UCS4����
	//************************************
	static int UTF8_To_UCS4(const byte* pbUTF8, DWORD& dwUCS4){
		int		i, iLen;
		byte	b;

		if (pbUTF8 == NULL){	// ��������
			return 0;
		}

		b = *pbUTF8++;
		if (b < 0x80){
			dwUCS4 = b;
			return 1;
		}

		if (b < 0xC0 || b > 0xFD){	// �Ƿ�UTF8
			return 0;
		}

		if (b < 0xE0){
			dwUCS4 = b & 0x1F;
			iLen = 2;
		}
		else if (b < 0xF0){
			dwUCS4 = b & 0x0F;
			iLen = 3;
		}
		else if (b < 0xF8){
			dwUCS4 = b & 7;
			iLen = 4;
		}
		else if (b < 0xFC){
			dwUCS4 = b & 3;
			iLen = 5;
		}
		else{
			dwUCS4 = b & 1;
			iLen = 6;
		}

		for (i = 1; i < iLen; i++){
			b = *pbUTF8++;
			if (b < 0x80 || b > 0xBF){	// �Ƿ�UTF8
				break;
			}

			dwUCS4 = (dwUCS4 << 6) + (b & 0x3F);
		}

		if (i < iLen){	// �Ƿ�UTF8
			return 0;
		}
		else{
			return iLen;
		}
	}

	//************************************
	// Method:    UCS4_To_UTF16
	// FullName:  che::HCodeConversion::UCS4_To_UTF16
	// Access:    public static 
	// Returns:   int	0����Ч��UCS4���� 1��ת����1��UTF16���� 2��ת����2��UTF16����
	// Qualifier: ��UCS4����ת����UTF16����
	// Parameter: DWORD dwUCS4		Ҫת����UCS4����
	// Parameter: WORD * pwUTF16	���ڴ洢ת�����UTF16���롣��ΪNULL�����Ի�ȡ������Ϣ���ַ�����
	//************************************
	static int UCS4_To_UTF16(DWORD dwUCS4, WORD* pwUTF16){
		if (dwUCS4 <= 0xFFFF){
			if (pwUTF16 != NULL){
				*pwUTF16 = static_cast<WORD>(dwUCS4);
			}

			return 1;
		}
		else if (dwUCS4 <= 0xEFFFF){
			if (pwUTF16 != NULL){
				pwUTF16[0] = static_cast<WORD>(0xD800 + (dwUCS4 >> 10) - 0x40);	// ��10λ
				pwUTF16[1] = static_cast<WORD>(0xDC00 + (dwUCS4 & 0x03FF));		// ��10λ
			}

			return 2;
		}
		else{
			return 0;
		}
	}

	//************************************
	// Method:    UTF16_To_UCS4
	// FullName:  che::HCodeConversion::UTF16_To_UCS4
	// Access:    public static 
	// Returns:   int 0�������������Ч��UTF16���� 1��1��UTF16���뱻ת�� 2��2��UTF16���뱻ת��
	// Qualifier: ��UTF16����ת����UCS4����
	// Parameter: const WORD * pwUTF16 ��Ҫת����UTF16����
	// Parameter: DWORD & dwUCS4 �洢ת�����UCS4����
	//************************************
	static int UTF16_To_UCS4(const WORD* pwUTF16, DWORD& dwUCS4){
		WORD	w1, w2;

		if (pwUTF16 == NULL){	// ��������
			return 0;
		}

		w1 = pwUTF16[0];
		if (w1 >= 0xD800 && w1 <= 0xDFFF){	// �������������Surrogate Area��
			if (w1 < 0xDC00){
				w2 = pwUTF16[1];
				if (w2 >= 0xDC00 && w2 <= 0xDFFF){
					dwUCS4 = (w2 & 0x03FF) + (((w1 & 0x03FF) + 0x40) << 10);
					return 2;
				}
			}

			return 0;	// �Ƿ�UTF16����	
		}
		else{
			dwUCS4 = w1;
			return 1;
		}
	}

	//************************************
	// Method:    UTF8Str_To_UTF16Str
	// FullName:  che::HCodeConversion::UTF8Str_To_UTF16Str
	// Access:    public static 
	// Returns:   int 0��ת��ʧ�� >0��UTF16�ַ�������
	// Qualifier: ��UTF8�ַ���ת����UTF16�ַ���
	// Parameter: const byte * pbszUTF8Str ��Ҫת����UTF8�ַ���
	// Parameter: WORD * pwszUTF16Str �洢ת�����UTF16�ַ�������ΪNULL�����Ի�ȡ���賤����Ϣ���ַ�����
	//************************************
	static int UTF8Str_To_UTF16Str(const byte* pbszUTF8Str, WORD* pwszUTF16Str){
		int		iNum, iLen;
		DWORD	dwUCS4;

		if (pbszUTF8Str == NULL){	// ��������
			return 0;
		}

		iNum = 0;	// ͳ����Ч�ַ�����
		while (*pbszUTF8Str){	// UTF8����ת��ΪUCS4����
			iLen = UTF8_To_UCS4(pbszUTF8Str, dwUCS4);
			if (iLen == 0){	// �Ƿ���UTF8����
				return 0;
			}

			pbszUTF8Str += iLen;

			// UCS4����ת��ΪUTF16����
			iLen = UCS4_To_UTF16(dwUCS4, pwszUTF16Str);
			if (iLen == 0){
				return 0;
			}

			if (pwszUTF16Str != NULL){
				pwszUTF16Str += iLen;
			}

			iNum += iLen;
		}

		if (pwszUTF16Str != NULL){
			*pwszUTF16Str = 0;	// д���ַ����������
		}

		return iNum;
	}

	//************************************
	// Method:    UTF16Str_To_UTF8Str
	// FullName:  che::HCodeConversion::UTF16Str_To_UTF8Str
	// Access:    public static 
	// Returns:   int 0��ת��ʧ�� >0��UTF8�ַ������ȣ�������NULL�ַ���
	// Qualifier: ��UTF16�ַ���ת����UTF8�ַ���
	// Parameter: const WORD * pwszUTF16Str	��Ҫת����UTF16�ַ���
	// Parameter: byte * pbszUTF8Str		�洢ת�����UTF8�ַ�������ΪNULL�����Ի�ȡ���賤����Ϣ���ֽ�����
	//************************************
	static int UTF16Str_To_UTF8Str(const WORD* pwszUTF16Str, byte* pbszUTF8Str){
		int		iNum, iLen;
		DWORD	dwUCS4;

		if (pwszUTF16Str == NULL){	// ��������
			return 0;
		}

		iNum = 0;
		while (*pwszUTF16Str){	// UTF16����ת��ΪUCS4����
			iLen = UTF16_To_UCS4(pwszUTF16Str, dwUCS4);
			if (iLen == 0){	// �Ƿ���UTF16����
				return 0;
			}

			pwszUTF16Str += iLen;

			// UCS4����ת��ΪUTF8����
			iLen = UCS4_To_UTF8(dwUCS4, pbszUTF8Str);
			if (iLen == 0){
				return 0;
			}

			if (pbszUTF8Str != NULL){
				pbszUTF8Str += iLen;
			}

			iNum += iLen;
		}

		if (pbszUTF8Str != NULL){
			*pbszUTF8Str = 0;	// д���ַ����������
		}

		return iNum;
	}
};
NAMESPACE_END_CHE
#endif // HCodeConversion_H__
