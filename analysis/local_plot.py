import ROOT
import os
import sys

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(False)

import argparse
parser = argparse.ArgumentParser(description='Plot local run HB stuff')
parser.add_argument("--run", type=int, help="Run number")
args = parser.parse_args()

f = ROOT.TFile("hbanalysis_{}.root".format(args.run), "READ")

c_adcvsts = ROOT.TCanvas("c_adcvsts", "c_adcvsts", 800, 600)
f.Get("hbAnalysis/ADCvsTS").Draw("colz")
c_adcvsts.SaveAs("c_adcvsts_run{}.pdf".format(args.run))

for depth in xrange(1, 5):
	c_sumq = ROOT.TCanvas("c_meansumq_depth{}".format(depth), "Mean sumQ", 800, 600)
	f.Get("hbAnalysis/MeanSumQ_depth{}".format(depth)).Draw("colz")
	c_sumq.SaveAs("{}_run{}.pdf".format(c_sumq.GetName(), args.run))

