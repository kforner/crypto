#include <string.h>
#include <gmpxx.h>
/*
 *    p=134078079299425970995740249982058461274793658205923933 \77723561443721764030073546976801874298166903427690031 \858186486050853753882811946569946433649006084171
 *    g=11717829880366207009516117596335367088558084999998952205 \59997945906392949973658374667057217647146031292859482967 \5428279466566527115212748467589894601965568
 *    h=323947510405045044356526437872806578864909752095244 \952783479245297198197614329255807385693795855318053 \2878928001494706097394108577585732452307673444020333
 */

#include <iostream>
#include <sstream>
#include <unordered_map>
using namespace std;

typedef std::unordered_map<std::string,unsigned int> stringintmap;

int main(int argc, char **argv) {

	const char* ps = "13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084171";
	const char* gs = "11717829880366207009516117596335367088558084999998952205599979459063929499736583746670572176471460312928594829675428279466566527115212748467589894601965568";
	const char* hs = "3239475104050450443565264378728065788649097520952449527834792452971981976143292558073856937958553180532878928001494706097394108577585732452307673444020333";

	mpz_class p(ps), g(gs), h(hs);
	cout << "p=" << p << endl;
	cout << "g=" << g << endl;
	cout << "h=" << h << endl;

	mpz_class ginv;
	mpz_class pm2 = p - 2;
	mpz_powm(ginv.get_mpz_t(), g.get_mpz_t(), pm2.get_mpz_t(), p.get_mpz_t());

	unsigned int B = 1<<20; // 2^20


	mpz_class gb;
	mpz_powm_ui(gb.get_mpz_t(), g.get_mpz_t(), B, p.get_mpz_t());



	stringintmap hash;
	mpz_class g1(1);

	mpz_class u;
	for(unsigned int i = 0; i < B; ++i) {
		u = (h * g1) % p;
		hash.insert ( {u.get_str(), i } );
		g1 = (ginv*g1)%p;
	}

	unsigned int x0 = 0, x1 = 0;
	bool found = false;
	stringintmap::const_iterator it;
	mpz_class g0(1);
	for(unsigned int i = 0; i <= B; ++i) {

//		if ( i < 20 )
//				cerr << "g0="<< g0 << endl;

		it = hash.find( g0.get_str());
		if ( it != hash.end() ) {
//			cerr << "key=" << it->first << endl;
			x1 = it->second;
			x0 = i;
			found = true;
			break;
		}
		g0 = (g0*gb)%p;

	}

	if ( found )  {

		mpz_class x = x0*mpz_class(B) + x1;
		mpz_class h2;
		mpz_powm(h2.get_mpz_t(), g.get_mpz_t(), x.get_mpz_t(), p.get_mpz_t());
		cout << "x0=" << x0 << "x1=" << x1 << "x=" << x << endl;

		cout << "h2=" << h2 << endl;

	} else {
		cout << "not found\n";

	}





	// test: g^(p-1) % p should equal 1
//	mpz_class p1 = p -1 ;
//	mpz_powm(gbc, g.get_mpz_t(), p1.get_mpz_t(), p.get_mpz_t());
//	mpz_class one(gbc);
//	cout << "one=" << one << endl;



}


