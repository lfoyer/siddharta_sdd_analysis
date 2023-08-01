/**
 * Filter elements (small gaussians)
 * @param vector Vector to filter
 * @return Filtered vector with only even main elements
 **/
Double_t* filterElem(std::vector<Double_t> vector)
{
    Double_t* vectorMainElements = new Double_t[size(calibrationMainElements)];

    for (Int_t i = 0, j = 0; i < vector.size(); i++)
    {
        if (calibrationMainElements.count(calibrationElements[i]))
        {
            vectorMainElements[j] = vector[i];
            j++;
        }
    }

    return vectorMainElements;
}



        
