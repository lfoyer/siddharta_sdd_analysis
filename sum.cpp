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
#include "get/getEvHistograms.cpp"
#include "get/getHistograms.cpp"
#include "get/getId.cpp"
#include "other/clearHistogram.cpp"
#include "other/filterElem.cpp"
#include "other/saveToJson.cpp"
#include "other/scaleXaxis.cpp"
#include "other/sumHistograms.cpp"
#include "plot/sumResidualFunction.cpp"
#include "plot/sumFitHistogram.cpp"
#include "plot/sumSigmaFunction.cpp"

/**
 * Sum + fit
 **/
int sum()
{
	TFile *file = new TFile("output/graphs.root", "READ");
	std::vector<TH1F *> evHists = getEvHistograms(file);
	TH1F *sumHist = sumHistograms(evHists);
	TFile outputFile("output/sum.root", "RECREATE");
	sumHist->Write("sum");

	// Fit parameters
	fitParams sumParameters;
	sumParameters.rangeMin = 4000;
	sumParameters.rangeMax = 18000;
	sumParameters.backgroundSlope = 1;
	sumParameters.backgroundOffset = 2000;

	// Set initial parameters for fit (values.cpp file)
	for (string element : elements)
	{
		sumParameters.initialValues.sigmas.push_back(sum_sig[element]);
		sumParameters.initialValues.amplitudes.push_back(sum_amp[element]);
		sumParameters.initialValues.means.push_back(eV[element]);
	}

	// Make fit
	TCanvas *fitCanvas = new TCanvas("fitCanvas", "fitCanvas", 800, 600);
	gaussValues fittedHistogram = sumFitHistogram(sumHist, sumParameters);
	fitCanvas->Write("fitCanvas");
	sumHist->Write("sumFitted");

	// Residuals
	TCanvas *residualCanvas = new TCanvas();
	TGraph *sumResidual = sumResidualFunction(fittedHistogram.means, fittedHistogram.meanErrors, sumParameters.initialValues.means);
	residualCanvas->Write("residualCanvas");
	residualCanvas->SetWindowPosition(800, 0);

	// FWHM
	TCanvas *sigmaCanvas = new TCanvas();
	TGraphErrors *sumSigma = sumSigmaFunction(fittedHistogram.sigmas, fittedHistogram.sigmaErrors, fittedHistogram.means);
	sigmaCanvas->Write("fwhmCanvas");
	sigmaCanvas->SetWindowPosition(0, 1000);


	// Get name
	// std::string name = histogram->GetName();

	/* // Elaborate sigma function
	(new TCanvas())->SetWindowPosition(800, 0);
	Int_t n = size(elements);
	TGraphErrors *sigmaGraph = new TGraphErrors(n);
	sigmaGraph->SetTitle("sigmaGraph");

	for (Int_t i = 0; i < n; i++)
	{
		cout << "sigmas: " << fittedHistogram.sigmas[i] << endl;
		sigmaGraph->SetPoint(i, fittedHistogram.means[i], fittedHistogram.sigmas[i] * 2.35);
		sigmaGraph->SetPointError(i, 0, fittedHistogram.sigmaErrors[i] * 2.35);
	}

	sigmaGraph->SetMarkerColor(kBlue);
	sigmaGraph->SetMarkerStyle(kFullCircle);
	sigmaGraph->Draw("AP");

	// Define fit function
	TF1 *sigmaFitFunc = new TF1("sigmaFitFunc", sigmaFitFunction, 4000, 16000, 2);
	sigmaFitFunc->SetParameter(0, 0.01);
	sigmaFitFunc->SetParameter(1, 90);
	sigmaFitFunc->SetParLimits(0, 0.001, 0.1);
	sigmaFitFunc->SetParLimits(1, 50, 150);

	for (Int_t i = 0; i < n; i++)
	{
		TLatex *label = new TLatex(fittedHistogram.means[i], fittedHistogram.sigmas[i]* 2.35 + 4, Form("#splitline{%s}{%.1f eV, %.1f sig}", elements[i].c_str(), fittedHistogram.means[i], fittedHistogram.sigmas[i] * 2.35));
		label->SetTextSize(0.01);
		label->SetTextColor(kRed);
		label->SetTextAlign(22);
		label->Draw();
	}

	sigmaGraph->Fit(sigmaFitFunc, "R");
	sigmaGraph->Write("sumSigmaGraph");



	// very lazy copy paste
	std::vector<Double_t> mea = {fittedHistogram.means[0], fittedHistogram.means[4], fittedHistogram.means[7], fittedHistogram.means[14]};
	std::vector<Double_t> sig = {fittedHistogram.sigmas[0], fittedHistogram.sigmas[4], fittedHistogram.sigmas[7], fittedHistogram.sigmas[14]};
	std::vector<Double_t> sigErr = {fittedHistogram.sigmaErrors[0], fittedHistogram.sigmaErrors[4], fittedHistogram.sigmaErrors[7], fittedHistogram.sigmaErrors[14]};


	(new TCanvas())->SetWindowPosition(0, 800);
	TGraphErrors *sigmaGraph2 = new TGraphErrors(n);
	sigmaGraph2->SetTitle("sigmaGraph2");

	for (Int_t i = 0; i < 4; i++)
	{
		cout << "sigmas: " << sig[i] << endl;
		sigmaGraph2->SetPoint(i, mea[i], sig[i] * 2.35);
		sigmaGraph2->SetPointError(i, 0, sigErr[i] * 2.35);
	}

	sigmaGraph2->SetMarkerColor(kBlue);
	sigmaGraph2->SetMarkerStyle(kFullCircle);
	sigmaGraph2->Draw("AP");

	std::vector<string >filterElems = {elements[0], elements[4], elements[7], elements[14]};

	for (Int_t i = 0; i < 4; i++)
	{
		TLatex *label = new TLatex(mea[i], sig[i]* 2.35 + 4, Form("#splitline{%s}{%.1f eV, %.1f sig}", filterElems[i].c_str(), mea[i], sig[i] * 2.35));
		label->SetTextSize(0.01);
		label->SetTextColor(kRed);
		label->SetTextAlign(22);
		label->Draw();
	}

	sigmaGraph2->Set(4);

	sigmaGraph2->Fit(sigmaFitFunc, "R");
	sigmaGraph2->Write("sumSigmaGraph2");

	calibrationSettings fittedSigmaFunction;
    fittedSigmaFunction.slope = sigmaFitFunc->GetParameter(0);
    fittedSigmaFunction.intercept = sigmaFitFunc->GetParameter(1);

	// Labels for plot
    TLatex *label = new TLatex(10000, 180, Form("%f*x + %f", fittedSigmaFunction.slope, fittedSigmaFunction.intercept));
    label->SetTextSize(0.03);
    label->SetTextColor(kRed);
    label->SetTextAlign(22);
    label->Draw();



	// Lazy residuals plot
	(new TCanvas())->SetWindowPosition(800, 1000);

	TGraphErrors *residualGraph = new TGraphErrors(n);
	residualGraph->SetTitle("residualGraph");

	Double_t residuals[4];

	for (Int_t i = 0; i < 4; i++)
	{
		residuals[i] = (mea[i] * fittedSigmaFunction.slope + fittedSigmaFunction.intercept)- sig[i]*2.35;
		std::cout << "residual: " << residuals[i] << std::endl;
		residualGraph->SetPoint(i, mea[i], residuals[i]);
	}
	residualGraph->Set(4);
	residualGraph->SetMarkerColor(kRed);
	residualGraph->SetMarkerStyle(kFullCircle);
	residualGraph->Draw("AP"); */

	return 0;
}