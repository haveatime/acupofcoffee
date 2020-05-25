#ifndef OPENSSL__WRAP__H
#define OPENSSL__WRAP__H

#include <QObject>

#ifdef OPENSSL_WRAP_EXPORT
#define OPENSSL_WRAP_DECL Q_DECL_EXPORT
#else
#define OPENSSL_WRAP_DECL Q_DECL_IMPORT
#endif

#include <QByteArray>
#include <openssl/evp.h>

class OPENSSL_WRAP_DECL OpensslWrap
{
	public:

	/* 对称加密算法 */
	static bool encode_data_openssl_symmetric(const EVP_CIPHER *cipher, const QByteArray &openssl_key, const QByteArray &plain_text, QByteArray &encode_result, const char *debug_flag);
	static bool decode_data_openssl_symmetric(const EVP_CIPHER *cipher, const QByteArray &openssl_key, const QByteArray &encode_text, QByteArray &plain_result, const char *debug_flag);

	/* HASH摘要算法 */
	static bool digest_data_openssl_hash(const EVP_MD *md, const QByteArray &msg_text, QByteArray &dg_result, const char *debug_flag);

	/* 非对称加密算法 */
	static bool generate_privkey_openssl_asymmetric(int curve_nid, QByteArray &privkey, QByteArray &pubkey, const char *debug_flag);
	static bool generate_pubkey_openssl_asymmetric(int curve_nid, const QByteArray privkey, QByteArray &pubkey, const char *debug_flag);
	static bool signature_message_openssl_asymmetric(int curve_nid, const EVP_MD *md, const QByteArray msg_text, const QByteArray privkey, const QByteArray idname, QByteArray &sig_result, const char *debug_flag);
	static bool verify_signature_openssl_asymmetric(int curve_nid, const EVP_MD *md, const QByteArray msg_text, const QByteArray sig_text, const QByteArray pubkey, const QByteArray idname, const char *debug_flag);
	static bool encode_data_openssl_asymmetric(int curve_nid, bool(*set_param_func)(EVP_PKEY_CTX *ctx), const QByteArray &pub_key, const QByteArray &plain_text, QByteArray &encode_result, const char *debug_flag);
	static bool decode_data_openssl_asymmetric(int curve_nid, bool(*set_param_func)(EVP_PKEY_CTX *ctx), const QByteArray &priv_key, const QByteArray &encode_text, QByteArray &plain_result, const char *debug_flag);
};

#endif
