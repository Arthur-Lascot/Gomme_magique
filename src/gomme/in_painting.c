void inPainting(SDL_Surface surface, int* map)
{
    int w = surface->w;
    int h = surface->h;
    int C[w*h];
    //initialization of the array of confidence
    for(int i = 0; i<w*h; i++)
    {
        if(map[i]==0)
            C[i] = 1; // C(pixel) = 1 for pixel in (I - Omega)
        else
            C[i] = 0; // C(pixel) = 0 for pixel in (Omega)
    }// note à moi même avoir une aire superficiel de vrai aire + 1 contour de pixel pour bouger la bordure
}