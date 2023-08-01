#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TLatex.h>
#include <TMath.h>
#include <TH1.h>
#include <TSpectrum.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <THStack.h>
#include <TPolyMarker.h>
#include <string>

#include "data/structures.cpp"
#include "data/values.cpp"
#include "get/getHistograms.cpp"
#include "get/getId.cpp"
#include "other/clearHistogram.cpp"
#include "other/filterElem.cpp"
#include "other/saveToJson.cpp"
#include "other/scaleXaxis.cpp"
#include "other/sumHistograms.cpp"
#include "plot/conversionFunction.cpp"
#include "plot/fitHistogram.cpp"
#include "plot/conversionResidualFunction.cpp"
#include "plot/sigmaFunction.cpp"

Int_t calibrate()
{
    // Open file
	TFile *file = new TFile("data/20210317_1139_0317_2011_Lab_xray_30kv_6ua_140HV_board_6_9_661_all_hist.root", "READ");
	std::vector<TH1F *> histograms = getHistograms(file);

	// Prepare output file
	TFile outputFile("output/output.root", "RECREATE");
	if (!outputFile.IsOpen())
	{
		std::cerr << "Failed to open the ROOT file." << std::endl;
		return 1;
	}
	TDirectory *adcGraphs = outputFile.mkdir("adcGraph");
	TDirectory *conversionGraphs = outputFile.mkdir("conversionGraph");
	TDirectory *evGraphs = outputFile.mkdir("evGraph");
	TDirectory *residualGraphs = outputFile.mkdir("residualGraph");
	TDirectory *sigmaGraphs = outputFile.mkdir("sigmaGraph");

	// Set fit parameters
	fitParams parameters;
	parameters.rangeMin = 1500;
	parameters.rangeMax = 6000;
	parameters.backgroundSlope = 0.0019;
	parameters.backgroundOffset = 305;
	parameters.initialValues.sigmas = {sig["Ti_ka"], sig["Ti_kb"], sig["Cr_ka"], sig["Cr_kb"], sig["Fe_ka"], sig["Fe_kb"], sig["Cu_ka"], sig["Cu_kb"], sig["Sr_ka"], sig["Sr_kb"]};
	parameters.initialValues.amplitudes = {amp["Ti_ka"], amp["Ti_kb"], amp["Cr_ka"], amp["Cr_kb"], amp["Fe_ka"], amp["Fe_kb"], amp["Cu_ka"], amp["Cu_kb"], amp["Sr_ka"], amp["Sr_kb"]};
	parameters.numPeaks = 10;

	// Collection of histograms
	std::vector<gaussValues> fittedGaussValues;
	std::vector<calibrationSettings> calibrationSettingsList;

	// Index
	Int_t numHist = 0;

	// Iterate through all channels
	for (TH1F *histogram : histograms)
	{
		// Get name
		std::string name = histogram->GetName();

		// Fit select channel
		new TCanvas();
		gaussValues fittedHistogram = fitHistogram(histogram, parameters, numHist);
		fittedGaussValues.push_back(fittedHistogram);
		adcGraphs->cd();
		histogram->Write(Form("adc_%s", name.c_str()));

		// Elaborate ADC-eV conversion function
		(new TCanvas())->SetWindowPosition(800, 0);
		Int_t n = size(mainElements); // div by 2 to not count small gaussians
		Double_t *ADCEnergy = filterElem(fittedGaussValues[numHist].means);
		Double_t *ADCEnergyError = filterElem(fittedGaussValues[numHist].meanErrors);

		// Elaborate ADC to eV conversion function
		calibrationSettings calibrationFunction = conversionFunction(n, eVEnergy, ADCEnergy, parameters.rangeMin, parameters.rangeMax);
		calibrationFunction.sdd_id = getId(histogram);
		calibrationSettingsList.push_back(calibrationFunction);
		conversionGraphs->cd();
		calibrationFunction.graph->Write(Form("conversion_%s", name.c_str()));

		// Scale X-axis of spectrum to match to eV
		TH1F *eVHistogram = (TH1F *)histogram->Clone();
		eVHistogram->SetTitle(Form("Calibrated %s", histogram->GetName()));
		ScaleXaxis(eVHistogram, ScaleX, calibrationFunction.slope, calibrationFunction.intercept);
		clearHistogram(eVHistogram); // Remove fit and peak finder from plot
		evGraphs->cd();
		eVHistogram->Write(Form("ev_%s", name.c_str()));

		// Elaborate residual function
		(new TCanvas())->SetWindowPosition(0, 1000);
		TGraphErrors *residualGraph = conversionResidualFunction(n, calibrationFunction, eVEnergy, ADCEnergy, ADCEnergyError);
		residualGraphs->cd();
		residualGraph->Write(Form("residual_%s", name.c_str()));

		// Elaborate sigma function
		(new TCanvas())->SetWindowPosition(800, 1000);
		Double_t *ADCsigmas = filterElem(fittedGaussValues[numHist].sigmas);
		Double_t *ADCsigmasError = filterElem(fittedGaussValues[numHist].sigmaErrors);
		TGraphErrors *sigmaGraph = sigmaFunction(n, calibrationFunction, eVEnergy, ADCsigmas, ADCsigmasError);
		sigmaGraphs->cd();
		sigmaGraph->Write(Form("sigma_%s", name.c_str()));

		numHist++;
		break;
	}

	saveToJson(calibrationSettingsList);

	return 0;
}