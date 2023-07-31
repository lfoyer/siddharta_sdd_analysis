/* void tail()
{
    TF1 *f1 = new TF1("f1", "([3] / (2 * [0] * [2])) * TMath::Exp((((x) - [1]) / ([0] * [2])) + (1 / (2 * [0] * [0]))) * TMath::Erfc((x - [1]) / (TMath::Sqrt(2) * [2]) + (1 / TMath::Sqrt(2) * [0]))", 0, 30000);
    f1->SetParameter(0, 5);
    f1->SetParameter(1, 14097);
    f1->SetParameter(2, 20000);
    f1->SetParameter(3, -0.0001);
    f1->Draw();
}

Double_t stepFunction(Double_t x, Double_t threshold)
{
    return (x > threshold) ? 1 : 0;
}

Double_t fitFunction(Double_t *x, Double_t *par)
{
    return par[0] + par[1] * (*x) + par[2] * TMath::Exp(par[3] * (*x - 9100)) * stepFunction(*x, 9100);
} */

/* void linExp()
{
    TF1 *f1 = new TF1("f1", fitFunction, 0, 30000, 4);
    f1->SetParameter(0, 2137);
    f1->SetParameter(1, -0.227);
    f1->SetParameter(2, -16000);
    f1->SetParameter(3, 6.95);
    f1->Draw();
} */

/* Double_t funcTest(Double_t *x, Double_t *par)
{
    return 2399 + 0.853 * (*x) * stepFunction(*x, 9100) + 1000 * TMath::Exp(0.0006 * ((*x) - 3043)) * stepFunction(-*x, -9100);
} */

Double_t tailFunction(Double_t x, Double_t amplitude, Double_t beta, Double_t mean, Double_t sigma)
{
    return (amplitude / (2 * sigma * beta)) * TMath::Exp(((x - mean) / (beta * sigma)) + (1 / (2 * beta * beta))) * TMath::Erfc((x - mean) / (TMath::Sqrt(2) * sigma) + (1 / TMath::Sqrt(2) * beta));
}

Double_t tail(Double_t *x, Double_t *par)
{
    return tailFunction(*x, 328673*300, 1.5, 15770, 103.7);
}

Double_t gauss(Double_t *x, Double_t *par)
{
    return 328673 * TMath::Gaus(*x, 15770, 103.7);
}

Double_t sum(Double_t *x, Double_t *par)
{
    return tail(x, par) + gauss(x, par);
}

void test()
{
    TF1 *f1 = new TF1("f1", tail, 13000, 20000);
    TF1 *f2 = new TF1("f2", gauss, 13000, 20000);
    TF1 *f3 = new TF1("f3", sum, 13000, 20000);
    
    f3->Draw();
    f2->Draw("same");
    f1->Draw("same");
    f2->SetLineColor(kBlue);

    f3->SetLineColor(kGreen);
}