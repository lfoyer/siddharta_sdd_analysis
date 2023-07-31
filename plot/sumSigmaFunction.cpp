Double_t sigmaFitFunction(Double_t *x, Double_t *par)
{
    return TMath::Sqrt(par[0] * (*x) + par[1]);
}

TGraphErrors *sumSigmaFunction(std::vector<Double_t> fitSigmas, std::vector<Double_t> fitSigmasErrors, std::vector<Double_t> fitMeans)
{
    // TGraph
    TGraphErrors *sigmaGraph = new TGraphErrors(size(elemWithTail));
    sigmaGraph->SetTitle("FWHM");

    Int_t j = 0;

    for (Int_t i = 0; i < size(elements); i++)
    {
        if (elemWithTail.count(elements[i]))
        {
            sigmaGraph->SetPoint(j, fitMeans[i], fitSigmas[i] * 2.35);
            sigmaGraph->SetPointError(j, 0, fitSigmasErrors[i] * 2.35);
            j++;
        }
    }

    sigmaGraph->SetMarkerColor(kBlue);
    sigmaGraph->SetMarkerStyle(kFullCircle);

    // Define fit function
    TF1 *sigmaFitFunc = new TF1("sigmaFitFunc", sigmaFitFunction, -10000, 16500, 2);
    sigmaFitFunc->SetParameter(0, 0.007);
    sigmaFitFunc->SetParameter(1, 110);

    sigmaGraph->Fit(sigmaFitFunc, "R");

    sigmaGraph->Draw("AP");

    gPad->Update(); // This is important to update the range
    sigmaGraph->GetXaxis()->SetRangeUser(-10000, 16500);

    sigmaGraph->Draw("P same");

    // Retrieve function parameters
    calibrationSettings fittedSigmaFunction;
    fittedSigmaFunction.slope = sigmaFitFunc->GetParameter(0);
    fittedSigmaFunction.intercept = sigmaFitFunc->GetParameter(1);
    std::cout << "sqrt("<< fittedSigmaFunction.slope << "*x + " << fittedSigmaFunction.intercept << ")" << std::endl;

    // Labels for plot
    for (Int_t i = 0; i < size(fitSigmas); i++)
    {
        if (elemWithTail.count(elements[i]))
        {
            TLatex *label = new TLatex(fitMeans[i], fitSigmas[i] * 2.35 + 10, elements[i].c_str());
            label->SetTextSize(0.015);
            label->SetTextColor(kRed);
            label->SetTextAlign(22);
            label->Draw();
        }
    }

    // Fit function label
    TLatex *label = new TLatex(7000, 120, Form("sqrt(%f*x + %f)", fittedSigmaFunction.slope, fittedSigmaFunction.intercept));
    label->SetTextSize(0.015);
    label->SetTextColor(kRed);
    label->SetTextAlign(22);
    label->Draw();

    return sigmaGraph;
}