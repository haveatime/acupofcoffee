#include <openssl/md5.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/pem.h>
#include <openssl/sm2.h>

#include "opensslwrap.h"
#include "gmsslwrap.h"

#include <QDebug>

static int parse_asn1_get_length(const unsigned char **data, long *len)
{
    const unsigned char *p = *data;
    unsigned long ret = 0;
    unsigned long i;

    if (*p == 0x80)
    {
        ret = 0;
        p++;
    }
    else
    {
        i = *p & 0x7f;
        if (*(p++) & 0x80)
        {
            /* Skip leading zeroes */
            while (i && *p == 0)
            {
                p++;
                i--;
            }

            if (i > sizeof(long))
                return 0;

            while (i-- > 0)
            {
                ret <<= 8L;
                ret |= *(p++);
            }
        }
        else
            ret = i;
    }

    *data = p;
    *len = (long)ret;

    return 1;
}

static void parse_asn1_put_length(QByteArray &data, int length)
{
    int i, l;
    if (length <= 127)
        data += (char)length;
    else {
        l = length;
        for (i = 0; l > 0; i++)
            l >>= 8;
        data += (char)(i | 0x80);
        l = i;
				QByteArray append(i,(char)0);
        while (i-- > 0) {
            append[i] = (char)(length & 0xff);
            length >>= 8;
        }
				data += append;
    }
}

bool GmsslWrap::encode_data_gmsm4( const QByteArray &sm4_key, const QByteArray &plain_text, QByteArray &encode_result)
{
	char *debug_flag = getenv("GMSM4_DEBUG");

	const EVP_CIPHER *cipher = EVP_sms4_ecb();

	return OpensslWrap::encode_data_openssl_symmetric( cipher, sm4_key, plain_text, encode_result, debug_flag );
}

bool GmsslWrap::decode_data_gmsm4( const QByteArray &sm4_key, const QByteArray &encode_text, QByteArray &plain_result)
{
	char *debug_flag = getenv("GMSM4_DEBUG");

	const EVP_CIPHER *cipher = EVP_sms4_ecb();

	return OpensslWrap::decode_data_openssl_symmetric( cipher, sm4_key, encode_text, plain_result, debug_flag );
}

bool GmsslWrap::digest_data_gmsm3( const QByteArray &msg_text, QByteArray &dg_result)
{
	char *debug_flag = getenv("GMSM3_DEBUG");

	const EVP_MD *md = EVP_sm3();

	return OpensslWrap::digest_data_openssl_hash( md, msg_text, dg_result, debug_flag );
}

bool GmsslWrap::generate_privkey_gmsm2( QByteArray &privkey, QByteArray &pubkey )
{
	char *debug_flag = getenv("GMSM2_DEBUG");

	return OpensslWrap::generate_privkey_openssl_asymmetric( NID_sm2p256v1, privkey, pubkey, debug_flag );
}

bool GmsslWrap::generate_pubkey_gmsm2( const QByteArray privkey, QByteArray &pubkey )
{
	char *debug_flag = getenv("GMSM2_DEBUG");

	return OpensslWrap::generate_pubkey_openssl_asymmetric( NID_sm2p256v1, privkey, pubkey, debug_flag );
}

bool GmsslWrap::signature_message_gmsm2( const QByteArray msg_text, const QByteArray privkey, const QByteArray idname, QByteArray &sig_result )
{
	char *debug_flag = getenv("GMSM2_DEBUG");

	const EVP_MD *md = EVP_sm3();

	return OpensslWrap::signature_message_openssl_asymmetric(NID_sm2p256v1, md, msg_text, privkey, idname, &SM2_compute_id_digest, sig_result, debug_flag);
}

bool GmsslWrap::verify_signature_gmsm2( const QByteArray msg_text, const QByteArray sig_text, const QByteArray pubkey, const QByteArray idname )
{
	char *debug_flag = getenv("GMSM2_DEBUG");

	const EVP_MD *md = EVP_sm3();

	return OpensslWrap::verify_signature_openssl_asymmetric(NID_sm2p256v1, md, msg_text, sig_text, pubkey, idname, &SM2_compute_id_digest, debug_flag);
}

static bool encode_decode_set_param_gmsm2( EVP_PKEY_CTX *ctx )
{
	int ret;

	ret = EVP_PKEY_CTX_set_ec_scheme(ctx, NID_sm_scheme);
	if( 1 != ret )
	{
		qDebug() << "encode_decode_set_param_gmsm2 EVP_PKEY_CTX_set_ec_scheme error! ret=" << ret << endl;
		return false;
	}

  ret = EVP_PKEY_CTX_set_ec_encrypt_param(ctx, NID_sm3);
	if( 1 != ret )
	{
		qDebug() << "encode_decode_set_param_gmsm2 EVP_PKEY_CTX_set_ec_encrypt_param error! ret=" << ret << endl;
		return false;
	}

	return true;
}

bool GmsslWrap::encode_data_gmsm2( const QByteArray &sm2_key, const QByteArray &plain_text, QByteArray &encode_result)
{
	char *debug_flag = getenv("GMSM2_DEBUG");

	return OpensslWrap::encode_data_openssl_asymmetric( NID_sm2p256v1, &encode_decode_set_param_gmsm2, sm2_key, plain_text, encode_result, debug_flag );
}

bool GmsslWrap::decode_data_gmsm2( const QByteArray &sm2_key, const QByteArray &encode_text, QByteArray &plain_result)
{
	char *debug_flag = getenv("GMSM2_DEBUG");

	return OpensslWrap::decode_data_openssl_asymmetric( NID_sm2p256v1, &encode_decode_set_param_gmsm2, sm2_key, encode_text, plain_result, debug_flag );
}

bool GmsslWrap::gmsm2_encrypt_format_asn12plain( const QByteArray &asn1_format, const bool &hashbeforecipher, QByteArray &plain_format )
{
	int ret;
	unsigned char outbuf[1024];
	unsigned int outlen=1024, tmplen=1024;

	unsigned char *asn1_format_data = (unsigned char *)asn1_format.data();
	int asn1_format_len = asn1_format.size();

	if( asn1_format_data[0] != (char)0x30 )
	{
		qDebug() << "gmsm2_encrypt_format_asn12plain asn1_format_data[0] != 0x30 error!" << endl;
		return false;
	}

	unsigned char *sm2datapos = asn1_format_data+1;
	long sm2datalength;
	ret = parse_asn1_get_length( (const unsigned char **)&sm2datapos, &sm2datalength );
	if( ret <= 0)
	{
		qDebug() << "gmsm2_encrypt_format_asn12plain parse_asn1_get_length sm2datalength error!" << endl;
		return false;
	}

	unsigned char *sm2xbignumpos = sm2datapos;
	long sm2xbignumlength;
	QByteArray xbignumdata;
	if( sm2xbignumpos[0] != (char)0x02 )
	{
		qDebug() << "gmsm2_encrypt_format_asn12plain sm2xbignumpos[0] != 0x02 error!" << endl;
		return false;
	}
	sm2xbignumpos ++;
	ret = parse_asn1_get_length( (const unsigned char **)&sm2xbignumpos, &sm2xbignumlength );
	if( ret <= 0)
	{
		qDebug() << "gmsm2_encrypt_format_asn12plain parse_asn1_get_length sm2xbignumlength error!" << endl;
		return false;
	}
	if( sm2xbignumpos[0]==0 && sm2xbignumlength==33)
	{
		sm2xbignumpos ++;
		sm2xbignumlength --;
	}
	if( sm2xbignumlength != 32 )
	{
		qDebug() << "gmsm2_encrypt_format_asn12plain sm2xbignumlength != 32 error!" << endl;
		return false;
	}
	xbignumdata = QByteArray((char *)sm2xbignumpos,sm2xbignumlength);

	unsigned char *sm2ybignumpos = sm2xbignumpos+sm2xbignumlength;
	long sm2ybignumlength;
	QByteArray ybignumdata;
	if( sm2ybignumpos[0] != (char)0x02 )
	{
		qDebug() << "gmsm2_encrypt_format_asn12plain sm2ybignumpos[0] != 0x02 error!" << endl;
		return false;
	}
	sm2ybignumpos ++;
	ret = parse_asn1_get_length( (const unsigned char **)&sm2ybignumpos, &sm2ybignumlength );
	if( ret <= 0)
	{
		qDebug() << "gmsm2_encrypt_format_asn12plain parse_asn1_get_length sm2ybignumlength error!" << endl;
		return false;
	}
	if( sm2ybignumpos[0]==0 && sm2ybignumlength==33)
	{
		sm2ybignumpos ++;
		sm2ybignumlength --;
	}
	if( sm2ybignumlength != 32 )
	{
		qDebug() << "gmsm2_encrypt_format_asn12plain sm2ybignumlength != 32 error!" << endl;
		return false;
	}
	ybignumdata = QByteArray((char *)sm2ybignumpos,sm2ybignumlength);

	unsigned char *sm2hashpos = sm2ybignumpos+sm2ybignumlength;
	long sm2hashlength;
	QByteArray sm2hashdata;
	if( sm2hashpos[0] != (char)0x04 )
	{
		qDebug() << "gmsm2_encrypt_format_asn12plain sm2hashpos[0] != 0x04 error!" << endl;
		return false;
	}
	sm2hashpos ++;
	ret = parse_asn1_get_length( (const unsigned char **)&sm2hashpos, &sm2hashlength );
	if( ret <= 0)
	{
		qDebug() << "gmsm2_encrypt_format_asn12plain parse_asn1_get_length sm2hashlength error!" << endl;
		return false;
	}
	if( sm2hashlength != 32 )
	{
		qDebug() << "gmsm2_encrypt_format_asn12plain sm2hashlength != 32 error!" << endl;
		return false;
	}
	sm2hashdata = QByteArray((char *)sm2hashpos,sm2hashlength);

	unsigned char *sm2cipherpos = sm2hashpos+sm2hashlength;
	long sm2cipherlength;
	QByteArray sm2cipherdata;
	if( sm2cipherpos[0] != (char)0x04 )
	{
		qDebug() << "gmsm2_encrypt_format_asn12plain sm2cipherpos[0] != 0x04 error!" << endl;
		return false;
	}
	sm2cipherpos ++;
	ret = parse_asn1_get_length( (const unsigned char **)&sm2cipherpos, &sm2cipherlength );
	if( ret <= 0)
	{
		qDebug() << "gmsm2_encrypt_format_asn12plain parse_asn1_get_length sm2cipherlength error!" << endl;
		return false;
	}
	sm2cipherdata = QByteArray((char *)sm2cipherpos,sm2cipherlength);

	plain_format += (char)0x04;
	plain_format += xbignumdata;
	plain_format += ybignumdata;

	if( hashbeforecipher )
	{
		plain_format += sm2hashdata;
		plain_format += sm2cipherdata;
	}
	else
	{
		plain_format += sm2cipherdata;
		plain_format += sm2hashdata;
	}

	return true;
}

bool GmsslWrap::gmsm2_encrypt_format_plain2asn1( const QByteArray &plain_format, const bool &hashbeforecipher, QByteArray &asn1_format )
{
	int ret;
	unsigned char outbuf[1024];
	unsigned int outlen=1024, tmplen=1024;

	unsigned char *plain_format_data = (unsigned char *)plain_format.data();
	int plain_format_len = plain_format.size();

	if( plain_format_data[0] != (char)0x04 )
	{
		qDebug() << "gmsm2_encrypt_format_plain2asn1 plain_format_data[0] != 0x04 error!" << endl;
		return false;
	}

	unsigned char *sm2datapos;
	int sm2datalength;

	sm2datapos = plain_format_data + 1;
	sm2datalength = plain_format_len - 1;

	unsigned char *sm2xbignumpos = sm2datapos;
	int sm2xbignumlength = 32;
	QByteArray xbignumdata = QByteArray((char *)sm2xbignumpos,sm2xbignumlength);

	unsigned char *sm2ybignumpos = sm2datapos + sm2xbignumlength;
	int sm2ybignumlength = 32;
	QByteArray ybignumdata = QByteArray((char *)sm2ybignumpos,sm2ybignumlength);

	unsigned char *sm2hashpos;
	int sm2hashlength;
	QByteArray sm2hashdata;
	unsigned char *sm2cipherpos;
	int sm2cipherlength;
	QByteArray sm2cipherdata;

	sm2hashlength = 32;
	sm2cipherlength = sm2datalength-sm2xbignumlength-sm2ybignumlength-sm2hashlength;
	if( hashbeforecipher )
	{
		sm2hashpos = sm2ybignumpos + sm2ybignumlength;
		sm2hashdata = QByteArray((char *)sm2hashpos,sm2hashlength);

		sm2cipherpos = sm2hashpos + sm2hashlength;
		sm2cipherdata = QByteArray((char *)sm2cipherpos,sm2cipherlength);
	}
	else
	{
		sm2cipherpos = sm2ybignumpos + sm2ybignumlength;
		sm2cipherdata = QByteArray((char *)sm2cipherpos,sm2cipherlength);

		sm2hashpos = sm2cipherpos + sm2cipherlength;
		sm2hashdata = QByteArray((char *)sm2hashpos,sm2hashlength);
	}

	int fourdatalength = sm2datalength + 8;
	asn1_format += (char)0x30;
	parse_asn1_put_length(asn1_format, fourdatalength);

	asn1_format += (char)0x02;
	parse_asn1_put_length(asn1_format, sm2xbignumlength);
	asn1_format += xbignumdata;

	asn1_format += (char)0x02;
	parse_asn1_put_length(asn1_format, sm2ybignumlength);
	asn1_format += ybignumdata;

	asn1_format += (char)0x04;
	parse_asn1_put_length(asn1_format, sm2hashlength);
	asn1_format += sm2hashdata;

	asn1_format += (char)0x04;
	parse_asn1_put_length(asn1_format, sm2cipherlength);
	asn1_format += sm2cipherdata;

	return true;
}

void GmsslWrap::gmsm2_print_parameters( )
{
	int ret;
	unsigned char outbuf[1024];
	unsigned int outlen=1024, tmplen=1024;

	EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_sm2p256v1);
	if( NULL == group )
	{
		qDebug() << "gmsm2_print_parameters EC_GROUP_new_by_curve_name error!" << endl;
		return;
	}

/*
	BN_CTX *bctx = BN_CTX_new();
	if( NULL == bctx )
	{
		qDebug() << "gmsm2_print_parameters BN_CTX_new error!" << endl;
		EC_GROUP_free( group );
		return;
	}
*/

	BIGNUM *pbignum = BN_new();
	if( NULL == pbignum )
	{
		qDebug() << "gmsm2_print_parameters BN_new pbignum error!" << endl;
		EC_GROUP_free( group );
		return;
	}

	BIGNUM *abignum = BN_new();
	if( NULL == abignum )
	{
		qDebug() << "gmsm2_print_parameters BN_new abignum error!" << endl;
		EC_GROUP_free( group );
		BN_free( pbignum );
		BN_free( abignum );
		return;
	}

	BIGNUM *bbignum = BN_new();
	if( NULL == bbignum )
	{
		qDebug() << "gmsm2_print_parameters BN_new bbignum error!" << endl;
		EC_GROUP_free( group );
		BN_free( pbignum );
		BN_free( abignum );
		return;
	}

	ret = EC_GROUP_get_curve_GFp(group, pbignum, abignum, bbignum, NULL);
	if( 0 == ret )
	{
		qDebug() << "gmsm2_print_parameters EC_GROUP_get_curve_GFp error!" << endl;
		EC_GROUP_free( group );
		BN_free( pbignum );
		BN_free( abignum );
		BN_free( bbignum );
		return;
	}

	ret = BN_bn2bin(pbignum,outbuf);
	if( 0 == ret )
	{
		qDebug() << "gmsm2_print_parameters BN_bn2bin pbignum error!" << endl;
		EC_GROUP_free( group );
		BN_free( pbignum );
		BN_free( abignum );
		BN_free( bbignum );
		return;
	}

	QByteArray pbignumhex((const char *)outbuf,BN_num_bytes(pbignum));
	qDebug() << endl << "pbignumhex=" << pbignumhex.toHex() << endl;

	ret = BN_bn2bin(abignum,outbuf);
	if( 0 == ret )
	{
		qDebug() << "gmsm2_print_parameters BN_bn2bin abignum error!" << endl;
		EC_GROUP_free( group );
		BN_free( pbignum );
		BN_free( abignum );
		BN_free( bbignum );
		return;
	}

	QByteArray abignumhex((const char *)outbuf,BN_num_bytes(abignum));
	qDebug() << endl << "abignumhex=" << abignumhex.toHex() << endl;

	ret = BN_bn2bin(bbignum,outbuf);
	if( 0 == ret )
	{
		qDebug() << "gmsm2_print_parameters BN_bn2bin bbignum error!" << endl;
		EC_GROUP_free( group );
		BN_free( pbignum );
		BN_free( abignum );
		BN_free( bbignum );
		return;
	}

	QByteArray bbignumhex((const char *)outbuf,BN_num_bytes(bbignum));
	qDebug() << endl << "bbignumhex=" << bbignumhex.toHex() << endl;

	const EC_POINT *gpoint = EC_GROUP_get0_generator(group);
	if( NULL == gpoint )
	{
		qDebug() << "gmsm2_print_parameters EC_GROUP_get0_generator error!" << endl;
		EC_GROUP_free( group );
		BN_free( pbignum );
		BN_free( abignum );
		BN_free( bbignum );
		return;
	}

	BIGNUM *gbignum = BN_new();
	if( NULL == gbignum )
	{
		qDebug() << "gmsm2_print_parameters BN_new gbignum error!" << endl;
		EC_GROUP_free( group );
		BN_free( pbignum );
		BN_free( abignum );
		BN_free( bbignum );
		return;
	}

	const BIGNUM *gbignumret = EC_POINT_point2bn(group,gpoint,POINT_CONVERSION_UNCOMPRESSED,gbignum,NULL);
	if( NULL == gbignumret )
	{
		qDebug() << "gmsm2_print_parameters EC_POINT_point2bn error!" << endl;
		EC_GROUP_free( group );
		BN_free( pbignum );
		BN_free( abignum );
		BN_free( bbignum );
		BN_free( gbignum );
		return;
	}

	ret = BN_bn2bin(gbignum,outbuf);
	if( 0 == ret )
	{
		qDebug() << "gmsm2_print_parameters BN_bn2bin gbignum error!" << endl;
		EC_GROUP_free( group );
		BN_free( pbignum );
		BN_free( abignum );
		BN_free( bbignum );
		BN_free( gbignum );
		return;
	}

	QByteArray gbignumhex((const char *)outbuf,BN_num_bytes(gbignum));
	qDebug() << endl << "gbignumhex=" << gbignumhex.toHex() << endl;

	int sm2degree = EC_GROUP_get_degree(group);
	qDebug() << endl << "sm2degree=" << sm2degree << endl;

	BIGNUM *nbignum = BN_new();
	if( NULL == nbignum )
	{
		qDebug() << "gmsm2_print_parameters BN_new nbignum error!" << endl;
		EC_GROUP_free( group );
		BN_free( pbignum );
		BN_free( abignum );
		BN_free( bbignum );
		BN_free( gbignum );
		return;
	}

	ret = EC_GROUP_get_order(group, nbignum, NULL);
	if( 0 == ret )
	{
		qDebug() << "gmsm2_print_parameters EC_GROUP_get_order error!" << endl;
		EC_GROUP_free( group );
		BN_free( pbignum );
		BN_free( abignum );
		BN_free( bbignum );
		BN_free( gbignum );
		return;
	}

	ret = BN_bn2bin(nbignum,outbuf);
	if( 0 == ret )
	{
		qDebug() << "gmsm2_print_parameters BN_bn2bin orderbignum error!" << endl;
		EC_GROUP_free( group );
		BN_free( pbignum );
		BN_free( abignum );
		BN_free( bbignum );
		BN_free( gbignum );
		return;
	}

	QByteArray nbignumhex((const char *)outbuf,BN_num_bytes(nbignum));
	qDebug() << endl << "nbignumhex=" << nbignumhex.toHex() << endl;

	EC_GROUP_free( group );
	BN_free( pbignum );
	BN_free( abignum );
	BN_free( bbignum );
	BN_free( gbignum );
	BN_free( nbignum );
}


