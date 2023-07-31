TH1F *sumHistograms(std::vector<TH1F *> histograms)
{
    Double_t xMin = histograms[0]->GetXaxis()->GetXmin();
    Double_t xMax = histograms[0]->GetXaxis()->GetXmax();
    Int_t numBins = histograms[0]->GetNbinsX();

    for (TH1F *histogram : histograms)
    {
        xMin = std::min(xMin, histogram->GetXaxis()->GetXmin());
        xMax = std::max(xMax, histogram->GetXaxis()->GetXmax());
        numBins = std::max(numBins, histogram->GetNbinsX());
    }

    // Create the sum histogram
    TH1F *sumHist = new TH1F("sumHist", "Sum of Spectra", numBins, xMin, xMax);

    // Add the bin contents and errors from hist to the sum histogram
    for (TH1F *histogram : histograms)
    {
        for (Int_t bin = 1; bin <= histogram->GetNbinsX(); ++bin)
        {
            Double_t xValue = histogram->GetBinCenter(bin);
            Int_t sumBin = sumHist->FindBin(xValue);

            sumHist->AddBinContent(sumBin, histogram->GetBinContent(bin));
        }
    }

    sumHist->SetTitle("Sum");
    //sumHist->Draw("hist");
    std::cout << "Sum complete." << std::endl;
    return sumHist;
}