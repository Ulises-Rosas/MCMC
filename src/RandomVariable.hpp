#ifndef RandomVariable_hpp
#define RandomVariable_hpp

class RandomVariable {

    public:
               RandomVariable(void);
               RandomVariable(int x);
        double uniformRv(void);
        double uniformRv(double lower, double upper);
        double exponentialRv(double lambda);

    protected:
        int    seed;

};

#endif