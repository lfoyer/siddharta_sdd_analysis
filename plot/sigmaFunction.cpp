Double_t sigmaFitFunction(Double_t *, Double_t *);
Double_t sqrtSigmaFitFunction(Double_t *, Double_t *);

/**
 * Plot function of sigma values in regards to eV energy
 * @param n Number of values
 * @param fittedFunction Linear fit function of conversion
 * @param eVEnergy Theoretical energy values expressed in eV
 * @param sigmaValues Sigma values
 **/
TGraphErrors *sigmaFunction(Int_t n, calibrationSettings fittedFunction, Double_t *eVEnergy, Double_t *sigmaValues, Double_t *sigmasError)
{
    // TGraph
    TGraphErrors *sigmaGraph = new TGraphErrors(n);
    sigmaGraph->SetTitle("sigmaGraph");

    for (Int_t i = 0; i < n; i++)
    {
        cout << "sigmas: " << sigmaValues[i] << endl;
        sigmaGraph->SetPoint(i, eVEnergy[i], (sigmaValues[i] * fittedFunction.slope) * 2.35);
        sigmaGraph->SetPointError(i, 0, sigmasError[i] * fittedFunction.slope * 2.35);
    }

    sigmaGraph->SetMarkerColor(kBlue);
    sigmaGraph->SetMarkerStyle(kFullCircle);
    sigmaGraph->Draw("AP");

    // Define fit function
    TF1 *sigmaFitFunc = new TF1("sigmaFitFunc", sigmaFitFunction, 4000, 15000, 2);
    sigmaFitFunc->SetParameter(0, 0.007);
    sigmaFitFunc->SetParameter(1, 110);

    sigmaGraph->Fit(sigmaFitFunc, "R");

    // Retrieve function parameters
    calibrationSettings fittedSigmaFunction;
    fittedSigmaFunction.slope = sigmaFitFunc->GetParameter(0);
    fittedSigmaFunction.intercept = sigmaFitFunc->GetParameter(1);
    std::cout << fittedSigmaFunction.slope << "*x + " << fittedSigmaFunction.intercept << std::endl;

	for (Int_t i = 0; i < n; i++)
	{
		TLatex *label = new TLatex(eVEnergy[i], (sigmaValues[i] * fittedFunction.slope) * 2.35 + 10, Form("#splitline{%s}{%.1f eV, %.1f sig}", mainElements[i].c_str(), eVEnergy[i], (sigmaValues[i] * fittedFunction.slope) * 2.35));
		label->SetTextSize(0.015);
		label->SetTextColor(kRed);
		label->SetTextAlign(22);
		label->Draw();
	}

    // Labels for plot
    TLatex *label = new TLatex(7000, 120, Form("%f*x + %f", fittedSigmaFunction.slope, fittedSigmaFunction.intercept));
    label->SetTextSize(0.03);
    label->SetTextColor(kRed);
    label->SetTextAlign(22);
    label->Draw();

    // Lazy copy for sqrt...
    TF1 *sqrtSigmaFitFunc = new TF1("sqrtFitFunc", sqrtSigmaFitFunction, 4000, 15000, 2);
    sqrtSigmaFitFunc->SetParameter(0, 0.0091);
    sqrtSigmaFitFunc->SetParameter(1, 100);
    sqrtSigmaFitFunc->Draw("same");

    // Adjust axis
    //sigmaGraph->GetYaxis()->SetRangeUser(0, 200);
    //sigmaGraph->GetXaxis()->SetRangeUser(1500, 15000);

    return sigmaGraph;
}

/**
 * Sigma fit function
 * @param x x variable of function
 * @param par Parameters of fit (linear function parameters)
 * @return Fit function according to inputted parameters
 */
Double_t sigmaFitFunction(Double_t *x, Double_t *par)
{
    return par[0] * (*x) + par[1];
}

/**
 * Square root of sigma fit function
 * @param x x variable of function
 * @param par Parameters of fit (linear function parameters)
 * @return Fit function according to inputted parameters
 */
Double_t sqrtSigmaFitFunction(Double_t *x, Double_t *par)
{
    Double_t fitValue = sigmaFitFunction(x, par);
    return TMath::Sqrt(fitValue);
}
