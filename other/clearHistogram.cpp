void clearHistogram(TH1F *histogram)
{
    // Clear peak finder markers
    TList *functions = histogram->GetListOfFunctions();
    TPolyMarker *pm = (TPolyMarker *)functions->FindObject("TPolyMarker");
    functions->Remove(pm);

    // Clear fit function (finds function starting with "fitFunc")
    TIter nextFunc(histogram->GetListOfFunctions());
    TObject* obj;
    while ((obj = nextFunc())) {
        TString funcName = obj->GetName();
        if (funcName.BeginsWith("fitFunc")) {
            histogram->GetListOfFunctions()->Remove(obj);
            break;  // remove break if several functions
        }
    }
}
