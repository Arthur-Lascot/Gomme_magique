void inPainting(SDL_Surface surface, int* map)
{
    int w = surface->w;
    int h = surface->h;
    int C[w*h];
    for(int i = 0; i<w*h; i++)
    {
        if(map[i]==0)
            C[i] = 1;
        else
            C[i] = 0;
    }
}
