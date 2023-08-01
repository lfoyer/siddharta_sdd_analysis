/** Plot residual function of conversion function (real vs fit)
 * @param n Number of values
 * @param fittedFunction Linear fit function of conversion
 * @param eVEnergy Theoretical energy values expressed in eV
 * @param ADCEnergy Energy values expressed in ADC arbitrary units
 * @param ADCEnergyError Energy value error expressed in ADC arbitrary units
 **/
TGraphErrors* conversionResidualFunction(Int_t n, calibrationSettings fittedFunction, Double_t *eVEnergy, Double_t *ADCEnergy, Double_t *ADCEnergyError)
{

	// TGraph
	TGraphErrors *residualGraph = new TGraphErrors(n);
	residualGraph->SetTitle("residualGraph");

	Double_t residuals[n];

	for (Int_t i = 0; i < n; i++)
	{
		residuals[i] = eVEnergy[i] - (ADCEnergy[i] * fittedFunction.slope + fittedFunction.intercept);
		std::cout << "residual: " << residuals[i] << std::endl;
		residualGraph->SetPoint(i, eVEnergy[i], residuals[i]);
		residualGraph->SetPointError(i, 0, ADCEnergyError[i] * fittedFunction.slope * 2.35);
	}

	residualGraph->SetMarkerColor(kRed);
	residualGraph->SetMarkerStyle(kFullCircle);
	residualGraph->Draw("AP");

	for (Int_t i = 0; i < n; i++)
	{
		TLatex *label = new TLatex(eVEnergy[i] + 300, residuals[i] + 0.3, mainElements[i].c_str());
		label->SetTextSize(0.04);
		label->SetTextColor(kRed);
		label->SetTextAlign(22);
		label->Draw();
	}

	TF1 *zero = new TF1("zero", "0", 0, 16500);
	zero->SetLineStyle(2);
	zero->SetLineColor(kBlack);
	zero->Draw("same");

	gPad->SetGridy();

	return residualGraph;
}