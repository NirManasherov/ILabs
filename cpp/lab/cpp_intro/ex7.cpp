int main(void)
{
    float *f = new float(12.6); 
    delete f; 
    
    f = new float[15]; 
    delete[] f; 
}
