/*
 * BBGHibe.h
 *
 *  Created on: Dec 21, 2014
 *      Author: imiers
 */

#ifndef SRC_BBGHIBE_H_
#define SRC_BBGHIBE_H_
#include <cereal/types/base_class.hpp>
#include <cereal/access.hpp>

#include "relic_wrapper/relic_api.h"

#include "forwardsec.h"

class BbghPrivatekey{
public:
	PairingGroup group;
	G2 a0;
	G2 a1;
	std::vector<G1> b;
	std::vector<G2> bG2;
};


class BbhHIBEPublicKey:  public virtual  baseKey{
public:
	unsigned int l;
	G2 hibeg1;
	G1 g3G1;
	G2 g3G2;
	std::vector<G1> hG1;
	std::vector<G2> hG2;
	friend bool operator==(const BbhHIBEPublicKey& x, const BbhHIBEPublicKey& y){
		return  ((baseKey)x == (baseKey)y &&
				x.l == y.l && x.hibeg1 == y.hibeg1 && x.g3G1 == y.g3G1 &&
				x.g3G2 == y.g3G2 && x.hG1 == y.hG1 && x.hG2 == y.hG2);
	}
	friend bool operator!=(const BbhHIBEPublicKey& x, const BbhHIBEPublicKey& y){
		return !(x==y);
	}
	template <class Archive>
	  void serialize( Archive & ar )
	{
		ar(cereal::virtual_base_class<baseKey>(this),
				l,hibeg1,g3G1,g3G2,hG1,hG2);
	}
	friend class cereal::access;
};
//
namespace cereal
{
 template <class Archive>
 struct specialize<Archive, BbhHIBEPublicKey, cereal::specialization::member_serialize> {};
 // cereal no longer has any ambiguity when serializing MyDerived
}
class PartialBbghCT{
public:
	PairingGroup group;
	G1 B;
	G1 C;
	friend bool operator==(const PartialBbghCT& x,const PartialBbghCT& y){
		return x.B == y.B && x.C == y.C;
	}
};
class BbghCT: public PartialBbghCT{
public:
	BbghCT(const  PartialBbghCT & c) : PartialBbghCT(c){}
	GT A;
	friend bool operator==(const BbghCT& x,const BbghCT& y){
		return x.A == y.A && x.B == y.B && x.C == y.C; //FIXME call bass operator
	}
};

class Bbghibe
{
public:
	PairingGroup group;
	Bbghibe(){};
	~Bbghibe() {};

	void setup(const unsigned int & l, BbhHIBEPublicKey & pk, G2 & msk) const;

	void keygen(const BbhHIBEPublicKey & pk,const G2 & msk,const  std::vector<ZR> & id, BbghPrivatekey & sk) const;
    void keygen(const BbhHIBEPublicKey & pk,const  BbghPrivatekey & sk, const std::vector<ZR> & id,BbghPrivatekey & skout) const;

    PartialBbghCT blind(const BbhHIBEPublicKey & pk,const ZR &s, const  std::vector<ZR>  & id) const;
    BbghCT encrypt(const BbhHIBEPublicKey & pk, const GT & M, const std::vector<ZR>  & id ) const;

	GT recoverBlind(const BbghPrivatekey & sk, const PartialBbghCT & ct) const; // decrypt for PFSE
	GT decrypt(const BbghPrivatekey & sk,const BbghCT & ct) const; // actual decrypt

};


#endif /* SRC_BBGHIBE_H_ */
