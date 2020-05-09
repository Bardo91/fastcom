
#ifndef SERIALIZABLEVECTOR_H_
#define SERIALIZABLEVECTOR_H_


template<typename Type_>
class SerializableVector{
public:
    void push_back(Type_ _t){ v_.push_back(_t);};

    typename std::vector<Type_>::iterator begin() { return v_.begin();};
    typename std::vector<Type_>::iterator end() { return v_.end();};

    typename std::vector<Type_>::const_iterator begin() const { return v_.begin();};
    typename std::vector<Type_>::const_iterator end() const { return v_.end();};

    friend std::ostream & operator<<(std::ostream &_out, SerializableVector<Type_> &_v) {
        std::string res((char*)_v.data(), sizeof(Type_)*_v.size());
        _out << res;
        return _out;
    }
    friend std::istream & operator>>(std::istream &_in, SerializableVector<Type_> &_v) {
        
        char *buffer = new char[sizeof(float)];
        while (_in) {
            _in.read(buffer, sizeof(float));
            _v.push_back(*((float*)buffer));
        }

        delete[] buffer;
    }

    void resize(unsigned int _s) { v_.resize(_s);};

    Type_ *data() { return v_.data(); };

    size_t size() { return v_.size(); };

private:
    std::vector<Type_> v_;

};


#endif