#ifndef HBAnalysis_h
#define HBAnalysis_h

#include "FWCore/Framework/interface/global/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "EventFilter/HcalRawToDigi/interface/HcalUnpacker.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"
#include "DQM/HcalCommon/interface/ElectronicsMap.h"

#include "DQM/HcalCommon/interface/Utilities.h"

#include <string>
#include <map>
#include <vector>
#include <TTree.h>
#include <TSpectrum.h>
#include <TF1.h>
#include <TMath.h>

class HBAnalysis : public edm::EDAnalyzer {

public:
	HBAnalysis(const edm::ParameterSet& ps);// : pset(iConfig) {}
	~HBAnalysis();
	
	void beginJob();
	void analyze(const edm::Event&, const edm::EventSetup&);
	void endJob();
	
public:
	edm::ESHandle<HcalDbService> _dbService;
	edm::Service<TFileService> _fs;
	edm::InputTag		_tagQIE11;
	edm::EDGetTokenT<QIE11DigiCollection> _tokQIE11;
	static const double _adc2fC[];
	static const std::map<HcalSubdetector, TString> _subdet_to_string;

	HcalElectronicsMap const* _emap;
	hcaldqm::electronicsmap::ElectronicsMap _ehashmap;

	bool _first;
};

#endif
