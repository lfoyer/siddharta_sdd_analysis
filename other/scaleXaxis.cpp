// Courtesy of Pepe_le_Pew

/**
 * Convert histogram X-axis from ADC units to eV
 * @param histogram Histogram to adjust
 * @param slope Slope of ADC to eV conversion function
 * @param y_intercept Y-intercept of ADC to eV conversion function
 */
Double_t ScaleX(Double_t x, Double_t slope, Double_t intercept)
{
	Double_t v;
	v = slope * x + intercept; // "linear scaling" function example
	return v;
}

void ScaleAxis(TAxis *a, Double_t (*Scale)(Double_t, Double_t, Double_t), Double_t slope, Double_t intercept)
{
	if (!a)
		return; // just a precaution
	if (a->GetXbins()->GetSize())
	{
		// an axis with variable bins
		// note: bins must remain in increasing order, hence the "Scale"
		// function must be strictly (monotonically) increasing
		TArrayD X(*(a->GetXbins()));
		for (Int_t i = 0; i < X.GetSize(); i++)
			X[i] = Scale(X[i], slope, intercept);
		a->Set((X.GetSize() - 1), X.GetArray()); // new Xbins
	}
	else
	{
		// an axis with fix bins
		// note: we modify Xmin and Xmax only, hence the "Scale" function
		// must be linear (and Xmax must remain greater than Xmin)
		a->Set(a->GetNbins(),
			   Scale(a->GetXmin(), slope, intercept),  // new Xmin
			   Scale(a->GetXmax(), slope, intercept)); // new Xmax
	}
	return;
}

void ScaleXaxis(TH1 *h, Double_t (*Scale)(Double_t, Double_t, Double_t), Double_t slope, Double_t intercept)
{
	if (!h)
		return; // just a precaution
	ScaleAxis(h->GetXaxis(), Scale, slope, intercept);
	return;
}
