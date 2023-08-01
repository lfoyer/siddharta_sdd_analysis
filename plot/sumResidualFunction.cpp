TGraphErrors *sumResidualFunction(std::vector<Double_t> fitMeans, std::vector<Double_t> fitMeansErrors, std::vector<Double_t> tableMeans)
{
	// TGraph
	TGraphErrors *residualGraph = new TGraphErrors(size(elemWithTail));
	residualGraph->SetTitle("Fit vs Table residualGraph");

	for (Int_t i = 0, j = 0; i < size(elements); i++)
	{
		if (elemWithTail.count(elements[i]))
		{
			residualGraph->SetPoint(i, fitMeans[i], fitMeans[i] - tableMeans[i]);
			residualGraph->SetPointError(i, 0, fitMeansErrors[i]);
			j++;
		}
	}

	residualGraph->SetMarkerColor(kRed);
	residualGraph->SetMarkerStyle(kFullCircle);
	residualGraph->Draw("AP");

	for (Int_t i = 0; i < size(elements); i++)
	{
		if (elemWithTail.count(elements[i]))
		{
			TLatex *label = new TLatex(fitMeans[i] + 200, fitMeans[i] - tableMeans[i] + 2, elements[i].c_str());
			label->SetTextSize(0.015);
			label->SetTextColor(kRed);
			label->SetTextAlign(22);
			label->Draw();
		}
	}

	TF1 *zero = new TF1("zero", "0", 0, 16500);
	zero->SetLineStyle(2);
	zero->SetLineColor(kBlack);
	zero->Draw("same");

	gPad->SetGridy();

	return residualGraph;
}