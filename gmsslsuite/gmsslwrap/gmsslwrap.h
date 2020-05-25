#ifndef GMSSL__WRAP__H
#define GMSSL__WRAP__H

#include <QObject>

#ifdef GMSSL_WRAP_EXPORT
#define GMSSL_WRAP_DECL Q_DECL_EXPORT
#else
#define GMSSL_WRAP_DECL Q_DECL_IMPORT
#endif

#include <QByteArray>

class GMSSL_WRAP_DECL GmsslWrap
{
	public:

	static void gmsm2_print_parameters( );

	/* ���ܽ��ܹ���4 */
	static bool encode_data_gmsm4( const QByteArray &sm4_key, const QByteArray &plain_text, QByteArray &encode_result);
	static bool decode_data_gmsm4( const QByteArray &sm4_key, const QByteArray &encode_text, QByteArray &plain_result);

	/* ����HASHֵ����3 */
	static bool digest_data_gmsm3( const QByteArray &msg_text, QByteArray &dg_result);

	/* ���ɹ���2��Կ˽Կ */
	static bool generate_privkey_gmsm2( QByteArray &privkey, QByteArray &pubkey );
	static bool generate_pubkey_gmsm2( const QByteArray privkey, QByteArray &pubkey );

	/* ǩ����ǩ����2 */
	static bool signature_message_gmsm2( const QByteArray msg_text, const QByteArray privkey, const QByteArray idname, QByteArray &sig_result );
	static bool verify_signature_gmsm2( const QByteArray msg_text, const QByteArray sig_text, const QByteArray pubkey, const QByteArray idname );

	/* ���ܽ��ܹ���2 */
	static bool encode_data_gmsm2( const QByteArray &sm2_key, const QByteArray &plain_text, QByteArray &encode_result);
	static bool decode_data_gmsm2( const QByteArray &sm2_key, const QByteArray &encode_text, QByteArray &plain_result);

	/* ����2���� ANS1/DER�����PLAIN�����໥ת�� */
	/****************************************
	* PLAIN�������ϵı����ʽ
	* hashbeforecipher true=X||Y||Hash||Cipher
	*                  false=X||Y||Cipher||Hash
	****************************************/
	static bool gmsm2_encrypt_format_asn12plain(const QByteArray &asn1_format, const bool &hashbeforecipher, QByteArray &plain_format);
	static bool gmsm2_encrypt_format_plain2asn1(const QByteArray &plain_format, const bool &hashbeforecipher, QByteArray &asn1_format);
};

#endif
