#include <openssl/md5.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/pem.h>

#include "opensslwrap.h"

#include <QDebug>

bool OpensslWrap::encode_data_openssl_symmetric(const EVP_CIPHER *cipher, const QByteArray &openssl_key, const QByteArray &plain_text, QByteArray &encode_result, const char *debug_flag)
{
	int ret;

	if( debug_flag )
	{
		qDebug() << endl << "encode_data_openssl_symmetric openssl_key=" << endl << openssl_key.toHex() << endl;
		qDebug() << endl << "encode_data_openssl_symmetric plain_text=" << endl << plain_text.toHex() << endl;
	}

	unsigned char *encode_key = (unsigned char *)openssl_key.data();
	unsigned char *encode_iv = NULL;
	unsigned char *encode_data = (unsigned char *)plain_text.data();
	int encode_len = plain_text.size();

	int outlen = encode_len*2>1024?encode_len*2:1024;
	unsigned char *outbuf = (unsigned char *)malloc(outlen);
	int tmplen = 1024;
	if( NULL == outbuf )
	{
		qDebug() << "encode_data_openssl_symmetric malloc error!" << endl;
		return false;
	}

	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	if( NULL == ctx )
	{
		qDebug() << "encode_data_openssl_symmetric EVP_CIPHER_CTX_new error!" << endl;
		free(outbuf);
		return false;
	}

	EVP_CIPHER_CTX_init(ctx);

	if( NULL == cipher )
	{
		qDebug() << "encode_data_openssl_symmetric cipher=NULL error!" << endl;
		free(outbuf);
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}

	ret = EVP_CipherInit_ex(ctx,cipher,NULL,encode_key,encode_iv,1);
	if( 0 == ret )
	{
		qDebug() << "encode_data_openssl_symmetric EVP_CipherInit_ex error! ret=" << ret;
		free(outbuf);
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}

	ret = EVP_CipherUpdate(ctx,outbuf,&outlen,encode_data,encode_len);
	if( 0 == ret )
	{
		qDebug() << "encode_data_openssl_symmetric EVP_CipherUpdate error! ret=" << ret;
		free(outbuf);
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}

	ret = EVP_CipherFinal_ex(ctx,outbuf+outlen,&tmplen);
	if( 0 == ret )
	{
		qDebug() << "encode_data_openssl_symmetric EVP_CipherFinal_ex error! ret=" << ret;
		free(outbuf);
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}

	outlen += tmplen;

	encode_result = QByteArray((const char *)outbuf, outlen);

	free(outbuf);
	EVP_CIPHER_CTX_cleanup(ctx);

	if( debug_flag )
	{
		qDebug() << endl << "encode_data_openssl_symmetric encode_result=" << endl << encode_result.toHex() << endl;
	}

	return true;
}

bool OpensslWrap::decode_data_openssl_symmetric( const EVP_CIPHER *cipher, const QByteArray &openssl_key, const QByteArray &encode_text, QByteArray &plain_result, const char *debug_flag )
{
	int ret;

	if( debug_flag )
	{
		qDebug() << endl << "decode_data_openssl_symmetric openssl_key=" << endl << openssl_key.toHex() << endl;
		qDebug() << endl << "decode_data_openssl_symmetric encode_text=" << endl << encode_text.toHex() << endl;
	}

	unsigned char *decode_key = (unsigned char *)openssl_key.data();
	unsigned char *decode_iv = NULL;
	unsigned char *decode_data = (unsigned char *)encode_text.data();
	int decode_len = encode_text.size();

	int outlen = decode_len*2>1024?decode_len*2:1024;
	unsigned char *outbuf = (unsigned char *)malloc(outlen);
	int tmplen = 1024;
	if( NULL == outbuf )
	{
		qDebug() << "decode_data_openssl_symmetric malloc error!" << endl;
		return false;
	}

	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	if( NULL == ctx )
	{
		free(outbuf);
		qDebug() << "decode_data_openssl_symmetric EVP_CIPHER_CTX_new error!" << endl;
		return false;
	}

	EVP_CIPHER_CTX_init(ctx);

	if( NULL == cipher )
	{
		qDebug() << "decode_data_openssl_symmetric cipher=NULL error!" << endl;
		free(outbuf);
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}

	ret = EVP_CipherInit_ex(ctx,cipher,NULL,decode_key,decode_iv,0);
	if( 0 == ret )
	{
		qDebug() << "decode_data_openssl_symmetric EVP_CipherInit_ex error! ret=" << ret;
		free(outbuf);
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}

	ret = EVP_CipherUpdate(ctx,outbuf,&outlen,decode_data,decode_len);
	if( 0 == ret )
	{
		qDebug() << "decode_data_openssl_symmetric EVP_CipherUpdate error! ret=" << ret;
		free(outbuf);
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}

	ret = EVP_CipherFinal_ex(ctx,outbuf+outlen,&tmplen);
	if( 0 == ret )
	{
		qDebug() << "decode_data_openssl_symmetric EVP_CipherFinal_ex error! ret=" << ret;
		free(outbuf);
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}

	outlen += tmplen;

	plain_result = QByteArray((const char *)outbuf,outlen);

	free(outbuf);
	EVP_CIPHER_CTX_cleanup(ctx);

	if( debug_flag )
	{
		qDebug() << endl << "decode_data_openssl_symmetric plain_result=" << endl << plain_result.toHex() << endl;
	}

	return true;
}

bool OpensslWrap::digest_data_openssl_hash( const EVP_MD *md, const QByteArray &msg_text, QByteArray &dg_result, const char *debug_flag )
{
	int ret;

	if( debug_flag )
	{
		qDebug() << endl << "digest_data_openssl_hash msg_text=" << endl << msg_text.toHex() << endl;
	}

	unsigned char *msg_data = (unsigned char *)msg_text.data();
	int msg_len = msg_text.size();

	unsigned char outbuf[1024];
	unsigned int outlen=1024, tmplen=1024;

	EVP_MD_CTX *ctx = EVP_MD_CTX_new();
	if( NULL == ctx )
	{
		qDebug() << "digest_data_openssl_hash EVP_MD_CTX_new error!" << endl;
		return false;
	}

	EVP_MD_CTX_init(ctx);

	if( NULL == md )
	{
		qDebug() << "digest_data_openssl_hash md=NULL error!" << endl;
		return false;
	}

	ret = EVP_DigestInit_ex(ctx,md,NULL);
	if( 0 == ret )
	{
		qDebug() << "digest_data_openssl_hash EVP_DigestInit_ex error! ret=" << ret;
		EVP_MD_CTX_free(ctx);
		return false;
	}

	ret = EVP_DigestUpdate(ctx,msg_data,msg_len);
	if( 0 == ret )
	{
		qDebug() << "digest_data_openssl_hash EVP_DigestUpdate error! ret=" << ret;
		EVP_MD_CTX_free(ctx);
		return false;
	}

	ret = EVP_DigestFinal_ex(ctx,outbuf,&outlen);
	if( 0 == ret )
	{
		qDebug() << "digest_data_openssl_hash EVP_DigestFinal_ex error! ret=" << ret;
		EVP_MD_CTX_free(ctx);
		return false;
	}

	dg_result = QByteArray((const char *)outbuf,outlen);

	EVP_MD_CTX_destroy(ctx);

	if( debug_flag )
	{
		qDebug() << endl << "digest_data_openssl_hash dg_result=" << endl << dg_result.toHex() << endl;
	}

	return true;	
}

bool OpensslWrap::generate_privkey_openssl_asymmetric( int curve_nid, QByteArray &privkey, QByteArray &pubkey, const char *debug_flag )
{
	int ret;

	unsigned char outbuf[1024];
	unsigned int outlen=1024, tmplen=1024;

	EVP_PKEY_CTX *pctx;
	EVP_PKEY_CTX *kctx;
	EVP_PKEY *pevpkey=NULL, *params = NULL;
	
	pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
	if( NULL == pctx )
	{
		qDebug() << "generate_privkey_openssl_asymmetric EVP_PKEY_CTX_new_id EVP_PKEY_EC error!" << endl;
		return false;
	}

	ret = EVP_PKEY_paramgen_init(pctx);
	if( 0 == ret )
	{
		qDebug() << "generate_privkey_openssl_asymmetric EVP_PKEY_paramgen_init error! ret=" << ret;
		EVP_PKEY_CTX_free(pctx);
		return false;
	}

	ret = EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, curve_nid);
	if( 0 == ret )
	{
		qDebug() << "generate_privkey_openssl_asymmetric EVP_PKEY_CTX_set_ec_paramgen_curve_nid error! ret=" << ret << "curve_nid=" << curve_nid;
		EVP_PKEY_CTX_free(pctx);
		return false;
	}

	ret = EVP_PKEY_paramgen(pctx, &params);
	if( 0 == ret )
	{
		qDebug() << "generate_privkey_openssl_asymmetric EVP_PKEY_paramgen error! ret=" << ret;
		EVP_PKEY_CTX_free(pctx);
		return false;
	}

	kctx = EVP_PKEY_CTX_new(params, NULL);
	if( NULL == kctx )
	{
		qDebug() << "generate_privkey_openssl_asymmetric EVP_PKEY_CTX_new params error!" << endl;
		EVP_PKEY_CTX_free(pctx);
		return false;
	}

	ret = EVP_PKEY_keygen_init(kctx);
	if( 0 == ret )
	{
		qDebug() << "generate_privkey_openssl_asymmetric EVP_PKEY_keygen_init error! ret=" << ret;
		EVP_PKEY_CTX_free(pctx);
		EVP_PKEY_CTX_free(kctx);
		return false;
	}

	ret = EVP_PKEY_keygen(kctx,&pevpkey);
	if( 0 == ret )
	{
		qDebug() << "generate_privkey_openssl_asymmetric EVP_PKEY_keygen error! ret=" << ret;
		EVP_PKEY_CTX_free(pctx);
		EVP_PKEY_CTX_free(kctx);
		return false;
	}

	if( NULL == pevpkey )
	{
		qDebug() << "generate_privkey_openssl_asymmetric EVP_PKEY_keygen error!" << endl;
		EVP_PKEY_CTX_free(pctx);
		EVP_PKEY_CTX_free(kctx);
		return false;
	}

	const EC_KEY *peckey = EVP_PKEY_get0_EC_KEY(pevpkey);
	if( NULL == peckey )
	{
		qDebug() << "generate_privkey_openssl_asymmetric EVP_PKEY_get1_EC_KEY error!" << endl;
		EVP_PKEY_CTX_free(pctx);
		EVP_PKEY_CTX_free(kctx);
		return false;
	}

	const BIGNUM *privbignum = EC_KEY_get0_private_key(peckey);
	if( NULL == privbignum )
	{
		qDebug() << "generate_privkey_openssl_asymmetric EC_KEY_get0_private_key error!" << endl;
		EVP_PKEY_CTX_free(pctx);
		EVP_PKEY_CTX_free(kctx);
		return false;
	}

	ret = BN_bn2bin(privbignum,outbuf);
	if( 0 == ret )
	{
		qDebug() << "generate_privkey_openssl_asymmetric BN_bn2bin privbignum error!" << endl;
		EVP_PKEY_CTX_free(pctx);
		EVP_PKEY_CTX_free(kctx);
		return false;
	}

	privkey = QByteArray((const char *)outbuf,BN_num_bytes(privbignum));

	const EC_POINT *pecpoint = EC_KEY_get0_public_key(peckey);
	if( NULL == pecpoint )
	{
		qDebug() << "generate_privkey_openssl_asymmetric EC_KEY_get0_public_key error!" << endl;
		EVP_PKEY_CTX_free(pctx);
		EVP_PKEY_CTX_free(kctx);
		return false;
	}

	const EC_GROUP *group = EC_KEY_get0_group(peckey);
	if( NULL == group )
	{
		qDebug() << "generate_privkey_openssl_asymmetric EC_KEY_get0_group error!" << endl;
		EVP_PKEY_CTX_free(pctx);
		EVP_PKEY_CTX_free(kctx);
		return false;
	}

/*
	BN_CTX *bctx = BN_CTX_new();
	if( NULL == bctx )
	{
		qDebug() << "generate_privkey_openssl_asymmetric BN_CTX_new error!" << endl;
		EVP_PKEY_CTX_free(pctx);
		EVP_PKEY_CTX_free(kctx);
		return false;
	}
*/

	BIGNUM *pubbignum = BN_new();
	if( NULL == pubbignum )
	{
		qDebug() << "generate_privkey_openssl_asymmetric BN_new error!" << endl;
		EVP_PKEY_CTX_free(pctx);
		EVP_PKEY_CTX_free(kctx);
		return false;
	}

	const BIGNUM *pubbignumret = EC_POINT_point2bn(group,pecpoint,POINT_CONVERSION_UNCOMPRESSED,pubbignum,NULL);
	if( NULL == pubbignumret )
	{
		qDebug() << "generate_privkey_openssl_asymmetric EC_POINT_point2bn error!" << endl;
		EVP_PKEY_CTX_free(pctx);
		EVP_PKEY_CTX_free(kctx);
		return false;
	}

	ret = BN_bn2bin(pubbignum,outbuf);
	if( 0 == ret )
	{
		qDebug() << "generate_privkey_openssl_asymmetric BN_bn2bin pubbignum error!" << endl;
		EVP_PKEY_CTX_free(pctx);
		EVP_PKEY_CTX_free(kctx);
		return false;
	}

	pubkey = QByteArray((const char *)outbuf,BN_num_bytes(pubbignum));

	BN_free( pubbignum );
	EVP_PKEY_CTX_free(pctx);
	EVP_PKEY_CTX_free(kctx);

	if( debug_flag )
	{
		qDebug() << endl << "generate_privkey_openssl_asymmetric privkey=" << endl << privkey.toHex() << endl;
		qDebug() << endl << "generate_privkey_openssl_asymmetric pubkey=" << endl << pubkey.toHex() << endl;
	}

	return true;	
}

bool OpensslWrap::generate_pubkey_openssl_asymmetric( int curve_nid, const QByteArray privkey, QByteArray &pubkey, const char *debug_flag )
{
	int ret;

	if( debug_flag )
	{
		qDebug() << endl << "generate_pubkey_openssl_asymmetric privkeyhex=" << endl << privkey.toHex() << endl;
	}

	unsigned char outbuf[1024];
	unsigned int outlen=1024, tmplen=1024;

	unsigned char *privkey_data = (unsigned char *)privkey.data();
	int privkey_len = privkey.size();

	BIGNUM *privbignum = BN_new();
	if( NULL == privbignum )
	{
		qDebug() << "generate_pubkey_openssl_asymmetric BN_new error!" << endl;
		return false;
	}

	const BIGNUM *privbignumret = BN_bin2bn((const unsigned char *)privkey_data,privkey_len,privbignum);
	if( NULL == privbignumret )
	{
		qDebug() << "generate_pubkey_openssl_asymmetric BN_bin2bn error!" << endl;
		BN_free( privbignum );
		return false;
	}

	EC_GROUP *group = EC_GROUP_new_by_curve_name(curve_nid);
	if( NULL == group )
	{
		qDebug() << "generate_pubkey_openssl_asymmetric EC_KEY_get0_group error! curve_nid=" << curve_nid << endl;
		BN_free( privbignum );
		return false;
	}

	EC_POINT *pecpoint = EC_POINT_new(group);
	if( NULL == pecpoint )
	{
		qDebug() << "generate_pubkey_openssl_asymmetric EC_POINT_new error!" << endl;
		BN_free( privbignum );
		EC_GROUP_free( group );
		return false;
	}

/*
	BN_CTX *bctx = BN_CTX_new();
	if( NULL == bctx )
	{
		qDebug() << "generate_pubkey_openssl_asymmetric BN_CTX_new error!" << endl;
		BN_free( privbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		return false;
	}
*/

	ret = EC_POINT_mul(group,pecpoint,privbignum,NULL,NULL,NULL);
	if( 0 == ret )
	{
		qDebug() << "generate_pubkey_openssl_asymmetric EC_POINT_mul error!" << endl;
		BN_free( privbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		return false;
	}

	BIGNUM *pubbignum = BN_new();
	if( NULL == pubbignum )
	{
		qDebug() << "generate_pubkey_openssl_asymmetric BN_new error!" << endl;
		BN_free( privbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		return false;
	}

	const BIGNUM *pubbignumret = EC_POINT_point2bn(group,pecpoint,POINT_CONVERSION_UNCOMPRESSED,pubbignum,NULL);
	if( NULL == pubbignumret )
	{
		qDebug() << "generate_pubkey_openssl_asymmetric EC_POINT_point2bn error!" << endl;
		BN_free( privbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		BN_free( pubbignum );
		return false;
	}

	ret = BN_bn2bin(pubbignum,outbuf);
	if( 0 == ret )
	{
		qDebug() << "generate_pubkey_openssl_asymmetric BN_bn2bin pubbignum error!" << endl;
		BN_free( privbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		BN_free( pubbignum );
		return false;
	}

	pubkey = QByteArray((const char *)outbuf,BN_num_bytes(pubbignum));

	if( debug_flag )
	{
		qDebug() << endl << "generate_pubkey_openssl_asymmetric pubkeyhex=" << endl << pubkey.toHex() << endl;
	}

	BN_free( privbignum );
	EC_GROUP_free( group );
	EC_POINT_free( pecpoint );
	BN_free( pubbignum );

	return true;	
}

bool OpensslWrap::signature_message_openssl_asymmetric( int curve_nid, const EVP_MD *md, const QByteArray msg_text, const QByteArray privkey, const QByteArray idname, QByteArray &sig_result, const char *debug_flag )
{
	int ret;

	if( debug_flag )
	{
		qDebug() << endl << "signature_message_openssl_asymmetric msg_text=" << endl << msg_text.toHex() << endl;
		qDebug() << endl << "signature_message_openssl_asymmetric privkey=" << endl << privkey.toHex() << endl;
		qDebug() << endl << "signature_message_openssl_asymmetric idname=" << endl << idname.toHex() << endl;
	}

	unsigned char *msg_text_data = (unsigned char *)msg_text.data();
	int msg_text_len = msg_text.size();
	unsigned char *privkey_data = (unsigned char *)privkey.data();
	int privkey_len = privkey.size();

	unsigned char outbuf[1024];
	unsigned int outlen=1024, tmplen=1024;

	BIGNUM *privbignum = BN_new();
	if( NULL == privbignum )
	{
		qDebug() << "signature_message_openssl_asymmetric BN_new error!" << endl;
		return false;
	}

	const BIGNUM *privbignumret = BN_bin2bn((const unsigned char *)privkey_data, privkey_len, privbignum);
	if( NULL == privbignumret )
	{
		qDebug() << "signature_message_openssl_asymmetric BN_bin2bn error!" << endl;
		BN_free( privbignum );
		return false;
	}

	EC_KEY *peckey = EC_KEY_new_by_curve_name(curve_nid);
	if( NULL == peckey )
	{
		qDebug() << "signature_message_openssl_asymmetric EC_KEY_new_by_curve_name error! curve_nid=" << curve_nid << endl;
		BN_free( privbignum );
		return false;
	}

	ret = EC_KEY_set_private_key(peckey, privbignum);
	if( 0 == ret )
	{
		qDebug() << "signature_message_openssl_asymmetric EC_KEY_set_public_key error!" << endl;
		BN_free( privbignum );
		EC_KEY_free( peckey );
		return false;
	}

	EVP_PKEY *pevppkey = EVP_PKEY_new();
	if( NULL == pevppkey )
	{
		qDebug() << "signature_message_openssl_asymmetric EVP_PKEY_new error!" << endl;
		BN_free( privbignum );
		EC_KEY_free( peckey );
		return false;
	}

	ret = EVP_PKEY_set1_EC_KEY(pevppkey, peckey);
	if( 0 == ret )
	{
		qDebug() << "signature_message_openssl_asymmetric EVP_PKEY_set1_EC_KEY error!" << endl;
		BN_free( privbignum );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		return false;
	}

	EVP_MD_CTX *ctx = EVP_MD_CTX_new();
	if( NULL == ctx )
	{
		qDebug() << "signature_message_openssl_asymmetric EVP_MD_CTX_new error!" << endl;
		BN_free( privbignum );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		return false;
	}

	EVP_MD_CTX_init(ctx);

	if( NULL == md )
	{
		qDebug() << "signature_message_openssl_asymmetric md=NULL error!" << endl;
		BN_free( privbignum );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_MD_CTX_free(ctx);
		return false;
	}

	ret = EVP_SignInit_ex(ctx,md,NULL);
	if( 0 == ret )
	{
		qDebug() << "signature_message_openssl_asymmetric EVP_SignInit_ex error! ret=" << ret;
		BN_free( privbignum );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_MD_CTX_free(ctx);
		return false;
	}

	ret = EVP_SignUpdate(ctx,msg_text_data,msg_text_len);
	if( 0 == ret )
	{
		qDebug() << "signature_message_openssl_asymmetric EVP_SignUpdate error! ret=" << ret;
		BN_free( privbignum );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_MD_CTX_free(ctx);
		return false;
	}

	ret = EVP_SignFinal(ctx,outbuf,&outlen,pevppkey);
	if( 0 == ret )
	{
		qDebug() << "signature_message_openssl_asymmetric EVP_DigestFinal_ex error! ret=" << ret;
		BN_free( privbignum );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_MD_CTX_free(ctx);
		return false;
	}

	sig_result = QByteArray((const char *)outbuf,outlen);

	BN_free( privbignum );
	EC_KEY_free( peckey );
	EVP_PKEY_free(pevppkey);
	EVP_MD_CTX_free(ctx);

	if( debug_flag )
	{
		qDebug() << endl << "signature_message_openssl_asymmetric sig_result=" << endl << sig_result.toHex() << endl;
	}

	return true;	
}

bool OpensslWrap::verify_signature_openssl_asymmetric( int curve_nid, const EVP_MD *md, const QByteArray msg_text, const QByteArray sig_text, const QByteArray pubkey, const QByteArray idname, const char *debug_flag )
{
	int ret;

	if( debug_flag )
	{
		qDebug() << endl << "verify_signature_openssl_asymmetric msg_text=" << endl << msg_text.toHex() << endl;
		qDebug() << endl << "verify_signature_openssl_asymmetric sig_text=" << endl << sig_text.toHex() << endl;
		qDebug() << endl << "verify_signature_openssl_asymmetric pubkey=" << endl << pubkey.toHex() << endl;
		qDebug() << endl << "verify_signature_openssl_asymmetric idname=" << endl << idname.toHex() << endl;
	}

	unsigned char *pubkey_data = (unsigned char *)pubkey.data();
	int pubkey_len = pubkey.size();
	unsigned char *msg_text_data = (unsigned char *)msg_text.data();
	int msg_text_len = msg_text.size();
	unsigned char *sig_text_data = (unsigned char *)sig_text.data();
	int sig_text_len = sig_text.size();

	unsigned char outbuf[1024];
	size_t outlen=1024, tmplen=1024;

	BIGNUM *pubbignum = BN_new();
	if( NULL == pubbignum )
	{
		qDebug() << "verify_signature_openssl_asymmetric BN_new error!" << endl;
		return false;
	}

	const BIGNUM *pubbignumret = BN_bin2bn((const unsigned char *)pubkey_data, pubkey_len, pubbignum);
	if( NULL == pubbignumret )
	{
		qDebug() << "verify_signature_openssl_asymmetric BN_bin2bn error!" << endl;
		BN_free( pubbignum );
		return false;
	}

	EC_GROUP *group = EC_GROUP_new_by_curve_name(curve_nid);
	if( NULL == group )
	{
		qDebug() << "verify_signature_openssl_asymmetric EC_GROUP_new_by_curve_name error! curve_nid=" << curve_nid << endl;
		BN_free( pubbignum );
		return false;
	}

	EC_POINT *pecpoint = EC_POINT_new(group);
	if( NULL == pecpoint )
	{
		qDebug() << "verify_signature_openssl_asymmetric EC_POINT_new error!" << endl;
		BN_free( pubbignum );
		EC_GROUP_free( group );
		return false;
	}

/*
	BN_CTX *bctx = BN_CTX_new();
	if( NULL == bctx )
	{
		qDebug() << "verify_signature_openssl_asymmetric BN_CTX_new error!" << endl;
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		return false;
	}
*/

	const EC_POINT *pecpointret = EC_POINT_bn2point(group,pubbignum,pecpoint,NULL);
	if( NULL == pecpointret )
	{
		qDebug() << "verify_signature_openssl_asymmetric EC_POINT_bn2point error!" << endl;
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		return false;
	}

	EC_KEY *peckey = EC_KEY_new_by_curve_name(curve_nid);
	if( NULL == peckey )
	{
		qDebug() << "verify_signature_openssl_asymmetric EC_KEY_new_by_curve_name error! curve_nid" << curve_nid << endl;
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		return false;
	}

	ret = EC_KEY_set_public_key(peckey, pecpoint);
	if( 0 == ret )
	{
		qDebug() << "verify_signature_openssl_asymmetric EC_KEY_set_public_key error!" << endl;
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		return false;
	}

	EVP_PKEY *pevppkey = EVP_PKEY_new();
	if( NULL == pevppkey )
	{
		qDebug() << "verify_signature_openssl_asymmetric EVP_PKEY_new error!" << endl;
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		return false;
	}

	ret = EVP_PKEY_set1_EC_KEY(pevppkey, peckey);
	if( 0 == ret )
	{
		qDebug() << "verify_signature_openssl_asymmetric EVP_PKEY_set1_EC_KEY error!" << endl;
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		return false;
	}

	EVP_MD_CTX *ctx = EVP_MD_CTX_new();
	if( NULL == ctx )
	{
		qDebug() << "verify_signature_openssl_asymmetric EVP_MD_CTX_new error!" << endl;
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		return false;
	}

	EVP_MD_CTX_init(ctx);

	if( NULL == md )
	{
		qDebug() << "verify_signature_openssl_asymmetric md=NULL error!" << endl;
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_MD_CTX_free(ctx);
		return false;
	}

	ret = EVP_VerifyInit_ex(ctx,md,NULL);
	if( 0 == ret )
	{
		qDebug() << "verify_signature_openssl_asymmetric EVP_VerifyInit_ex error! ret=" << ret;
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_MD_CTX_free(ctx);
		return false;
	}

	ret = EVP_VerifyUpdate(ctx,msg_text_data,msg_text_len);
	if( 0 == ret )
	{
		qDebug() << "verify_signature_openssl_asymmetric EVP_VerifyUpdate error! ret=" << ret;
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_MD_CTX_free(ctx);
		return false;
	}

	ret = EVP_VerifyFinal(ctx,sig_text_data,sig_text_len,pevppkey);
	if( 0 == ret )
	{
		qDebug() << "verify_signature_openssl_asymmetric EVP_VerifyFinal error! ret=" << ret;
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_MD_CTX_free(ctx);
		return false;
	}

	BN_free( pubbignum );
	EC_GROUP_free( group );
	EC_POINT_free( pecpoint );
	EC_KEY_free( peckey );
	EVP_PKEY_free(pevppkey);
	EVP_MD_CTX_free(ctx);

	return true;	
}

bool OpensslWrap::encode_data_openssl_asymmetric( int curve_nid, bool (*set_param_func)( EVP_PKEY_CTX *ctx ), const QByteArray &pub_key, const QByteArray &plain_text, QByteArray &encode_result, const char *debug_flag )
{
	int ret;

	if( debug_flag )
	{
		qDebug() << endl << "encode_data_openssl_asymmetric pub_key=" << endl << pub_key.toHex() << endl;
		qDebug() << endl << "encode_data_openssl_asymmetric plain_text=" << endl << plain_text.toHex() << endl;
	}

	unsigned char *pubkey_data = (unsigned char *)pub_key.data();
	int pubkey_len = pub_key.size();
	unsigned char *encode_data = (unsigned char *)plain_text.data();
	int encode_len = plain_text.size();

	size_t outlen = encode_len*2>1024?encode_len*2:1024;
	unsigned char *outbuf = (unsigned char *)malloc(outlen);
	size_t tmplen = 1024;
	if( NULL == outbuf )
	{
		qDebug() << "encode_data_openssl_asymmetric malloc error!" << endl;
		return false;
	}

	BIGNUM *pubbignum = BN_new();
	if( NULL == pubbignum )
	{
		qDebug() << "encode_data_openssl_asymmetric BN_new error!" << endl;
		free(outbuf);
		return false;
	}

	const BIGNUM *pubbignumret = BN_bin2bn((const unsigned char *)pubkey_data, pubkey_len, pubbignum);
	if( NULL == pubbignumret )
	{
		qDebug() << "encode_data_openssl_asymmetric BN_bin2bn error!" << endl;
		free(outbuf);
		BN_free( pubbignum );
		return false;
	}

	EC_GROUP *group = EC_GROUP_new_by_curve_name(curve_nid);
	if( NULL == group )
	{
		qDebug() << "encode_data_openssl_asymmetric EC_GROUP_new_by_curve_name error! curve_nid=" << curve_nid << endl;
		free(outbuf);
		BN_free( pubbignum );
		return false;
	}

	EC_POINT *pecpoint = EC_POINT_new(group);
	if( NULL == pecpoint )
	{
		qDebug() << "encode_data_openssl_asymmetric EC_POINT_new error!" << endl;
		free(outbuf);
		BN_free( pubbignum );
		EC_GROUP_free( group );
		return false;
	}

/*
	BN_CTX *bctx = BN_CTX_new();
	if( NULL == bctx )
	{
		qDebug() << "encode_data_openssl_asymmetric BN_CTX_new error!" << endl;
		free(outbuf);
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		return false;
	}
*/

	const EC_POINT *pecpointret = EC_POINT_bn2point(group,pubbignum,pecpoint,NULL);
	if( NULL == pecpointret )
	{
		qDebug() << "encode_data_openssl_asymmetric EC_POINT_bn2point error!" << endl;
		free(outbuf);
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		return false;
	}

	EC_KEY *peckey = EC_KEY_new_by_curve_name(curve_nid);
	if( NULL == peckey )
	{
		qDebug() << "encode_data_openssl_asymmetric EC_KEY_new_by_curve_name error! curve_nid=" << curve_nid << endl;
		free(outbuf);
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		return false;
	}

	ret = EC_KEY_set_public_key(peckey, pecpoint);
	if( 0 == ret )
	{
		qDebug() << "encode_data_openssl_asymmetric EC_KEY_set_public_key error!" << endl;
		free(outbuf);
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		return false;
	}

	EVP_PKEY *pevppkey = EVP_PKEY_new();
	if( NULL == pevppkey )
	{
		qDebug() << "encode_data_openssl_asymmetric EVP_PKEY_new error!" << endl;
		free(outbuf);
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		return false;
	}

	ret = EVP_PKEY_set1_EC_KEY(pevppkey, peckey);
	if( 0 == ret )
	{
		qDebug() << "encode_data_openssl_asymmetric EVP_PKEY_set1_EC_KEY error!" << endl;
		free(outbuf);
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		return false;
	}

	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pevppkey,NULL);
	if( NULL == ctx )
	{
		qDebug() << "encode_data_openssl_asymmetric EVP_PKEY_CTX_new error!" << endl;
		free(outbuf);
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		return false;
	}

	ret = EVP_PKEY_encrypt_init(ctx);
	if( 1 != ret )
	{
		qDebug() << "encode_data_openssl_asymmetric EVP_PKEY_encrypt_init error! ret=" << ret;
		free(outbuf);
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_PKEY_CTX_free(ctx);
		return false;
	}

	ret = (*set_param_func)(ctx);
	if( !ret )
	{
		qDebug() << "encode_data_openssl_asymmetric set_param_func return false error!" << endl;
		free(outbuf);
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_PKEY_CTX_free(ctx);
		return false;
	}

	ret = EVP_PKEY_encrypt(ctx,outbuf,&outlen,encode_data,encode_len);
	if( 1 != ret )
	{
		qDebug() << "encode_data_openssl_asymmetric EVP_PKEY_encrypt error! ret=" << ret;
		free(outbuf);
		BN_free( pubbignum );
		EC_GROUP_free( group );
		EC_POINT_free( pecpoint );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_PKEY_CTX_free(ctx);
		return false;
	}

	encode_result = QByteArray((const char *)outbuf,outlen);

	free(outbuf);
	BN_free( pubbignum );
	EC_GROUP_free( group );
	EC_POINT_free( pecpoint );
	EC_KEY_free( peckey );
	EVP_PKEY_free(pevppkey);
	EVP_PKEY_CTX_free(ctx);

	if( debug_flag )
	{
		qDebug() << endl << "encode_data_openssl_asymmetric encode_result=" << endl << encode_result.toHex() << endl;
	}

	return true;	
}

bool OpensslWrap::decode_data_openssl_asymmetric( int curve_nid, bool (*set_param_func)( EVP_PKEY_CTX *ctx ), const QByteArray &priv_key, const QByteArray &encode_text, QByteArray &plain_result, const char *debug_flag )
{
	int ret;

	if( debug_flag )
	{
		qDebug() << endl << "decode_data_openssl_asymmetric priv_key=" << endl << priv_key.toHex() << endl;
		qDebug() << endl << "decode_data_openssl_asymmetric encode_text=" << endl << encode_text.toHex() << endl;
	}

	unsigned char *priv_key_data = (unsigned char *)priv_key.data();
	int priv_key_len = priv_key.size();
	unsigned char *encode_text_data = (unsigned char *)encode_text.data();
	int encode_text_len = encode_text.size();

	size_t outlen = encode_text_len*2>1024?encode_text_len*2:1024;
	unsigned char *outbuf = (unsigned char *)malloc(outlen);
	size_t tmplen = 1024;
	if( NULL == outbuf )
	{
		qDebug() << "decode_data_openssl_asymmetric malloc error!" << endl;
		return false;
	}

	BIGNUM *privbignum = BN_new();
	if( NULL == privbignum )
	{
		qDebug() << "decode_data_openssl_asymmetric BN_new error!" << endl;
		free(outbuf);
		return false;
	}

	const BIGNUM *privbignumret = BN_bin2bn((const unsigned char *)priv_key_data, priv_key_len, privbignum);
	if( NULL == privbignumret )
	{
		qDebug() << "decode_data_openssl_asymmetric BN_bin2bn error!" << endl;
		free(outbuf);
		BN_free( privbignum );
		return false;
	}

	EC_KEY *peckey = EC_KEY_new_by_curve_name(curve_nid);
	if( NULL == peckey )
	{
		qDebug() << "decode_data_openssl_asymmetric EC_KEY_new_by_curve_name error! curve_nid=" << curve_nid << endl;
		free(outbuf);
		BN_free( privbignum );
		return false;
	}

	ret = EC_KEY_set_private_key(peckey, privbignum);
	if( 0 == ret )
	{
		qDebug() << "decode_data_openssl_asymmetric EC_KEY_set_public_key error!" << endl;
		free(outbuf);
		BN_free( privbignum );
		EC_KEY_free( peckey );
		return false;
	}

	EVP_PKEY *pevppkey = EVP_PKEY_new();
	if( NULL == pevppkey )
	{
		qDebug() << "decode_data_openssl_asymmetric EVP_PKEY_new error!" << endl;
		free(outbuf);
		BN_free( privbignum );
		EC_KEY_free( peckey );
		return false;
	}

	ret = EVP_PKEY_set1_EC_KEY(pevppkey, peckey);
	if( 0 == ret )
	{
		qDebug() << "decode_data_openssl_asymmetric EVP_PKEY_set1_EC_KEY error!" << endl;
		free(outbuf);
		BN_free( privbignum );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		return false;
	}

	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pevppkey,NULL);
	if( NULL == ctx )
	{
		qDebug() << "decode_data_openssl_asymmetric EVP_PKEY_CTX_new error!" << endl;
		free(outbuf);
		BN_free( privbignum );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		return false;
	}

	ret = EVP_PKEY_decrypt_init(ctx);
	if( 1 != ret )
	{
		qDebug() << "decode_data_openssl_asymmetric EVP_PKEY_decrypt_init error! ret=" << ret;
		free(outbuf);
		BN_free( privbignum );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_PKEY_CTX_free(ctx);
		return false;
	}

	ret = (*set_param_func)(ctx);
	if( !ret )
	{
		qDebug() << "decode_data_openssl_asymmetric set_param_func return false error!" << endl;
		free(outbuf);
		BN_free( privbignum );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_PKEY_CTX_free(ctx);
		return false;
	}

	ret = EVP_PKEY_decrypt(ctx,outbuf,&outlen,encode_text_data,encode_text_len);
	if( 1 != ret )
	{
		qDebug() << "decode_data_openssl_asymmetric EVP_PKEY_decrypt error! ret=" << ret;
		free(outbuf);
		BN_free( privbignum );
		EC_KEY_free( peckey );
		EVP_PKEY_free(pevppkey);
		EVP_PKEY_CTX_free(ctx);
		return false;
	}

	plain_result = QByteArray((const char *)outbuf,outlen);

	free(outbuf);
	BN_free( privbignum );
	EC_KEY_free( peckey );
	EVP_PKEY_free(pevppkey);
	EVP_PKEY_CTX_free(ctx);

	if( debug_flag )
	{
		qDebug() << endl << "decode_data_openssl_asymmetric plain_result=" << endl << plain_result.toHex() << endl;
	}

	return true;	
}
