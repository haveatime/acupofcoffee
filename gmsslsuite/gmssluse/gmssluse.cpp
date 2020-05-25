#include <QDebug>
#include <QByteArray>

#include "gmsslwrap.h"

void test_data_gmsm2( )
{
	bool operflag;
	qDebug() << endl << "*****************test_data_gmsm2*****************" << endl;

	QByteArray privkey;
	QByteArray pubkey;
	QByteArray pub2key;

	operflag = GmsslWrap::generate_privkey_gmsm2( privkey, pubkey );
	if( operflag )
		qDebug() << endl << "generate_privkey_gmsm2 success!";
	else
		qDebug() << endl << "generate_privkey_gmsm2 fail!";
	qDebug() << endl << "privkey=" << privkey.toHex() << endl;
	qDebug() << endl << "pubkey=" << pubkey.toHex() << endl;

	operflag = GmsslWrap::generate_pubkey_gmsm2( privkey, pub2key );
	if( operflag )
		qDebug() << endl << "generate_pubkey_gmsm2 success!";
	else
		qDebug() << endl << "generate_pubkey_gmsm2 fail!";
	qDebug() << endl << "pub2key=" << pub2key.toHex() << endl;

	QByteArray sig_text("signature message");
	QByteArray idname("yourname");
	QByteArray sig_result;

	qDebug() << endl << "sig_text=" << sig_text.toHex() << endl;
	qDebug() << endl << "idname=" << idname.toHex() << endl;

	operflag = GmsslWrap::signature_message_gmsm2( sig_text, privkey, idname, sig_result );
	if( operflag )
		qDebug() << endl << "signature_message_gmsm2 success!";
	else
		qDebug() << endl << "signature_message_gmsm2 fail!";

	qDebug() << endl << "sig_result=" << sig_result.toHex() << endl;

	operflag = GmsslWrap::verify_signature_gmsm2( sig_text, sig_result, pubkey, idname );
	if( operflag )
		qDebug() << endl << "verify_signature_gmsm2 success!";
	else
		qDebug() << endl << "verify_signature_gmsm2 fail!";

	QByteArray plain("1234567890abcdefghijklmnopqrstuv");
	QByteArray encrypt;

	qDebug() << endl << "plain=" << plain.toHex() << endl;

	operflag = GmsslWrap::encode_data_gmsm2( pubkey, plain, encrypt);
	if( operflag )
		qDebug() << endl << "encode_data_gmsm2 success!";
	else
		qDebug() << endl << "encode_data_gmsm2 fail!";

	qDebug() << endl << "encrypt=" << encrypt.toHex() << endl;

	QByteArray oldformat;
	operflag = GmsslWrap::gmsm2_encrypt_format_asn12plain( encrypt, false, oldformat);
	if( operflag )
		qDebug() << endl << "gmsm2_encrypt_format_asn12plain success!";
	else
		qDebug() << endl << "gmsm2_encrypt_format_asn12plain fail!";

	qDebug() << endl << "oldformat=" << oldformat.toHex() << endl;

	QByteArray decrypt;
	operflag = GmsslWrap::decode_data_gmsm2( privkey,encrypt, decrypt);
	if( operflag )
		qDebug() << endl << "decode_data_gmsm2 success!";
	else
		qDebug() << endl << "decode_data_gmsm2 fail!";

	qDebug() << endl << "decrypt=" << decrypt.toHex() << endl;

}

void test_data_gmsm3( )
{
	qDebug() << endl << "*****************test_data_gmsm3*****************" << endl;

	QByteArray mymsg("you are a strong man.");
	QByteArray mydg;

	qDebug() << endl << "message=" << mymsg.toHex() << endl;

	GmsslWrap::digest_data_gmsm3( mymsg, mydg);
	qDebug() << endl << "digest=" << mydg.toHex() << endl;
}

void test_data_gmsm4( )
{
	qDebug() << endl << "*****************test_data_gmsm4*****************" << endl;

	QByteArray sm4key("HELPabcd66875543");
	QByteArray encrypt;
	QByteArray plain("1234567890abcdef");
	QByteArray decrypt;

	qDebug() << endl << "sm4key=" << sm4key.toHex() << endl;
	qDebug() << endl << "plain=" << plain.toHex() << endl;

	GmsslWrap::encode_data_gmsm4( sm4key, plain, encrypt);
	qDebug() << endl << "encrypt=" << encrypt.toHex() << endl;

	GmsslWrap::decode_data_gmsm4( sm4key,encrypt, decrypt);
	qDebug() << endl << "decrypt=" << decrypt.toHex() << endl;
}


int main(int argc, char *argv[])
{
	test_data_gmsm2();

	test_data_gmsm3();

	test_data_gmsm4();

	return 0;
}
