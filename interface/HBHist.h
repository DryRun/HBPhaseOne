#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TH2D.h"

template<T>
class THcalHist {
public:
	THcalHist(TFileService* fs) {
		_fs = fs;
	};
	~THcalHist() {};

	void book(TFileService& fs);
	void fill(const HcalDetId& did);

private:
	TFileService* _fs;
	T* _h[7];

}

template<T>
THcalHist::book(TString name, TString title) {
	for (uint32 depth = 1; depth <= 7; ++depth) {
		_h[depth - 1] = _fs->make<T>()
	}
}