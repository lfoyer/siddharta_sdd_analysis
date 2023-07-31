/**
 * Filter out odd elements (small gaussians)
 * @param n Number of elements
 * @param vector Vector to filter
 * @return Filtered vector with only even indexed elements
 **/
Double_t* filterOdd(std::vector<Double_t> vector)
{
    Int_t size = vector.size() / 2;
    Double_t* vectorNoOdd = new Double_t[size];

    for (Int_t i = 0, j = 0; i < vector.size(); i += 2, ++j)
    {
        vectorNoOdd[j] = vector[i];
    }

    return vectorNoOdd;
}