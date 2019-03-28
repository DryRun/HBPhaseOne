#include "Analysis/HBPhaseOne/interface/HBAnalysis.h"

#include "FWCore/Framework/interface/ConstProductRegistry.h"
#include "FWCore/Framework/interface/ProductSelector.h"
#include "FWCore/Framework/interface/ProductSelectorRules.h"
#include "DataFormats/Provenance/interface/SelectedProducts.h"
#include "Math/LorentzVector.h"
#include "Math/Vector3D.h"

#include <map>
#include "boost/foreach.hpp"
#include <TBranch.h>
#include <TLorentzVector.h>

// --------------------------------------------------------------------------------------------------------
// Updated using the example here:
//		https://github.com/cms-sw/cmssw/blob/CMSSW_8_1_X/CalibTracker/SiStripCommon/plugins/ShallowTree.cc
//		https://github.com/cms-sw/cmssw/blob/CMSSW_8_1_X/CalibTracker/SiStripCommon/interface/ShallowTree.h
// --------------------------------------------------------------------------------------------------------

const double HBAnalysis::_adc2fC[] = {1.62, 4.86, 8.11, 11.35, 14.59, 17.84, 21.08, 24.32, 27.57, 30.81, 34.05, 37.30, 40.54, 43.78, 47.03, 50.27, 56.75, 63.24, 69.73, 76.21, 82.70, 89.19, 95.67, 102.2, 108.6, 115.1, 121.6, 128.1, 134.6, 141.1, 147.6, 154.0, 160.5, 167.0, 173.5, 180.0, 193.0, 205.9, 218.9, 231.9, 244.9, 257.8, 270.8, 283.8, 296.7, 309.7, 322.7, 335.7, 348.6, 361.6, 374.6, 387.6, 400.5, 413.5, 426.5, 439.4, 452.4, 478.4, 504.3, 530.3, 556.2, 582.1, 608.1, 634.0, 577.6, 603.0, 628.5, 654.0, 679.5, 705.0, 730.5, 756.0, 781.5, 806.9, 832.4, 857.9, 883.4, 908.9, 934.4, 959.9, 1010.9, 1061.8, 1112.8, 1163.8, 1214.8, 1265.7, 1316.7, 1367.7, 1418.7, 1469.6, 1520.6, 1571.6, 1622.6, 1673.5, 1724.5, 1775.5, 1826.5, 1877.5, 1928.4, 1979.4, 2081.4, 2183.3, 2285.3, 2387.2, 2489.2, 2591.1, 2693.1, 2795.0, 2897.0, 2998.9, 3100.9, 3202.8, 3304.8, 3406.8, 3508.7, 3610.7, 3712.6, 3814.6, 3916.5, 4018.5, 4120.4, 4324.3, 4528.2, 4732.1, 4936.1, 5140.0, 5343.9, 5547.8, 5331.9, 5542.5, 5753.1, 5963.7, 6174.3, 6384.9, 6595.5, 6806.1, 7016.7, 7227.3, 7437.9, 7648.4, 7859.0, 8069.6, 8280.2, 8490.8, 8912.0, 9333.2, 9754.3, 10175.5, 10596.7, 11017.9, 11439.1, 11860.3, 12281.4, 12702.6, 13123.8, 13545.0, 13966.2, 14387.3, 14808.5, 15229.7, 15650.9, 16072.1, 16493.2, 16914.4, 17756.8, 18599.1, 19441.5, 20283.9, 21126.2, 21968.6, 22811.0, 23653.3, 24495.7, 25338.0, 26180.4, 27022.8, 27865.1, 28707.5, 29549.9, 30392.2, 31234.6, 32076.9, 32919.3, 33761.7, 34604.0, 36288.8, 37973.5, 39658.2, 41342.9, 43027.6, 44712.4, 46397.1, 43321.6, 44990.1, 46658.6, 48327.1, 49995.7, 51664.2, 53332.7, 55001.2, 56669.7, 58338.2, 60006.7, 61675.2, 63343.7, 65012.3, 66680.8, 68349.3, 71686.3, 75023.3, 78360.3, 81697.4, 85034.4, 88371.4, 91708.4, 95045.4, 98382.5, 101719.5, 105056.5, 108393.5, 111730.6, 115067.6, 118404.6, 121741.6, 125078.6, 128415.7, 131752.7, 135089.7, 141763.8, 148437.8, 155111.8, 161785.9, 168459.9, 175134.0, 181808.0, 188482.1, 195156.1, 201830.1, 208504.2, 215178.2, 221852.3, 228526.3, 235200.4, 241874.4, 248548.4, 255222.5, 261896.5, 268570.6, 275244.6, 288592.7, 301940.8, 315288.9, 328637.0, 341985.1, 355333.1, 368681.2};

const std::map<HcalSubdetector, TString> HBAnalysis::_subdet_to_string = {
	{HcalBarrel, "HB"},
	{HcalEndcap, "HE"},
	{HcalForward, "HF"},
	{HcalOuter, "HO"}
};

HBAnalysis::HBAnalysis(const edm::ParameterSet& ps) {
	_tagQIE11 = ps.getUntrackedParameter<edm::InputTag>("tagQIE11", edm::InputTag("hcalDigis"));
	_tokQIE11 = consumes<QIE11DigiCollection>(_tagQIE11);	
}

HBAnalysis::~HBAnalysis() {
}

void HBAnalysis::beginJob() {
	_first = true;
	_events_processed = 0;

	_histograms["ADCvsTS"] = _fs->make<TH2F>("ADCvsTS", "ADCvsTS", 9, -0.5, 7.5, 256, -0.5, 255.5);
	((TH2F*)(_histograms["ADCvsTS"]))->GetXaxis()->SetTitle("TS");
	((TH2F*)(_histograms["ADCvsTS"]))->GetYaxis()->SetTitle("ADC");

	for (int depth = 1; depth <= 4; ++depth) {
		TString name = "MeanSumQ_depth";
		name += depth;
		_histograms[name] = _fs->make<TH2F>(name, name, 35, -17.5, 17.5, 74, -1.5, 72.5);
		((TH2F*)(_histograms[name]))->GetXaxis()->SetTitle("IEta");
		((TH2F*)(_histograms[name]))->GetYaxis()->SetTitle("IPhi");
	}
}

void HBAnalysis::analyze(const edm::Event& event, const edm::EventSetup& es) {
	// Load digis
	edm::Handle<QIE11DigiCollection> cQIE11;
	if (!event.getByToken(_tokQIE11, cQIE11)) {
		std::cout << "Collection QIE11DigiCollection isn't available" << _tagQIE11.label() << " " << _tagQIE11.instance() << std::endl;
		exit(1);
	}

	edm::ESHandle<HcalDbService> dbs;
	es.get<HcalDbRecord>().get(dbs);

	//if (_first) {
	//	_first = false;
	//	
	//	// Print the emap to verify if ngHB channels are present
	//	_emap = dbs->getHcalMapping();	
	//	_ehashmap.initialize(_emap, hcaldqm::electronicsmap::fD2EHashMap);
	//	_ehashmap.print();
	//}

	for (QIE11DigiCollection::const_iterator it = cQIE11->begin(); it != cQIE11->end(); ++it) {
		const QIE11DataFrame digi = static_cast<const QIE11DataFrame>(*it);
		HcalDetId const& did = digi.detid();
		//std::cout << "[debug] Digi " << did << std::endl;
		if ((did.subdet() != HcalBarrel)) {
			continue;
		}

		double sumq = 0.;
		for (int i=0; i<digi.samples(); i++) {
			sumq += _adc2fC[digi[i].adc()];
			//std::cout << "[debug] \t" << i << " => " << _adc2fC[digi[i].adc()] << std::endl;
		}

		TString hname = "MeanSumQ";
		hname += did.depth();
		((TH2F*)_histograms[hname])->Fill(did.ieta(), did.iphi(), sumq);

		if (sumq > 100. * digi.samples()) {
			for (int ts = 0; ts < digi.samples(); ++ts) {
				((TH2F*)_histograms["ADCvsTS"])->Fill(ts, digi[ts].adc());
			}
		}
	}

	_events_processed += 1;
}

void HBAnalysis::endJob() {
	((TH2F*)_histograms["MeanSumQ"])->Scale(1. / _events_processed);
}


