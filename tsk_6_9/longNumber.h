class longNumber{
    public:
        longNumber(string str);
        longNumber(vector<short> vector);
        longNumber(short sign, vector<short> vector);
        short sign() const;
        int length() const;
        string toString();
        short operator[](int index) const;
        longNumber operator+(const longNumber& number) const;
        longNumber operator-(const longNumber& number) const;
        void operator+=(const longNumber& number);
        void operator-=(const longNumber& number);
        longNumber operator<<(const int size) const;
        longNumber operator>>(const int size) const;
    protected:
        vector<short> digits;
        short sign;
};

longNumber::longNumber(string str){
    if(str[0] == '-')
        sign = -1;
    else if(str[0] == '0')sign = 0;
    else sign = 1;
    
    for(char digit:str)
        digits.insert(digits.end(), digit-'0');
}

longNumber::longNumber(vector<short> vector){
    sign = 1;
    digits = vector;
}

longNumber::longNumber(short sign, vector<short> vector){
    this->sign = sign;
    digits = vector;
}

int longNumber::length() const{
    return digits.size();}

short longNumber::sign() const{
    return sign;}

string longNumber::toString(){
    stringstream stream;
    for(short digit:digits)
        stream << digit;
    return stream.str();
}

short longNumber::operator[](int index) const{
    return digits[index];}

longNumber longNumber::operator+(const longNumber& number) const{
    vector<short> result;
    int length = digits.size(), numberLength = number.length(), increment=0;
    bool isIncrement = false;
    for(int i=0; i<numberLength; i++){
        short resultDigit = number[numberLength-i-1];
        if(isIncrement){
            resultDigit += increment;
            isIncrement = false;
            increment = 0;
        }
        if(i<length)
            resultDigit += digits[length-i-1];
        if(resultDigit>9){
            isIncrement = true;
            increment = resultDigit/10;
            resultDigit %= 10;
        }else isIncrement = false;
        result.insert(result.begin(), resultDigit);
    }
    for(int i=numberLength; i<length; i++){
        short resultDigit = digits[length-i-1];
        if(isIncrement){
            resultDigit += increment;
            isIncrement = false;
            increment = 0;
        }
        if(resultDigit>9){
            isIncrement = true;
            increment = resultDigit/10;
            resultDigit %= 10;
        }else isIncrement = false;
        result.insert(result.begin(), resultDigit);
    }
    if(isIncrement)
        result.insert(result.begin(), increment);
    return longNumber(result);
}

longNumber longNumber::operator-(const longNumber& number) const{
    vector<short> result;
    int length = digits.size(), numberLength = number.length(), decrement=1;
    bool isDecrement = false;
    for(int i=0; i<length; i++){
        short resultDigit = digits[length-i-1];
        if(isDecrement){
            resultDigit -= decrement;
            isDecrement = false;
        }
        if(i<numberLength)
            resultDigit -= number[numberLength-i-1];
        if(resultDigit<0){
            isDecrement = true;
            resultDigit += 10;
        }else isDecrement = false;
        result.insert(result.begin(), resultDigit);
    }
    while(result[0] == 0)
        result.erase(result.begin());
    for(int i=length; i<numberLength; i++){
        short resultDigit = - number[length-i-1];
        if(isDecrement){
            resultDigit -= decrement;
            isDecrement = false;
            decrement = 0;
        }
        if(resultDigit<0){
            isDecrement = true;
            decrement = 1;
            resultDigit += 10;
        }else isDecrement = false;
        result.insert(result.begin(), resultDigit);
    }
    return longNumber(result);
}

void longNumber::operator+=(const longNumber& number){
    *this = (number + (*this));}

void longNumber::operator-=(const longNumber& number){
    *this = (number - (*this));}

longNumber longNumber::operator<<(const int size) const{
    vector<short> values;
    for(int i=0; i<digits.size()-size; i++)
        values.insert(values.end(), digits[i]);
    return longNumber(values);
}

longNumber longNumber::operator>>(const int size) const{
    vector<short> values;
    for(int i=0; i<digits.size(); i++)
        values.insert(values.end(), digits[i]);
    for(int i=0; i<size; i++)
        values.insert(values.end(), 0);
    return longNumber(values);
}
