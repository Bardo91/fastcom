
#ifndef SERIALIZABLEMAT_H_
#define SERIALIZABLEMAT_H_

#include <vector>
#include <string_view>
#include <opencv2/opencv.hpp>

class SerializableMat{
public:
    SerializableMat(cv::Mat _img = cv::Mat(), int _jpegQuality = 50){
        imgRef_ = _img;
        params_.push_back(CV_IMWRITE_JPEG_QUALITY);
        params_.push_back(_jpegQuality);
    }

    operator const cv::Mat &() const { return imgRef_; };
    operator cv::Mat &() { return imgRef_; };
    const cv::Mat & getCvMat() const { return imgRef_; };

    friend std::ostream & operator<<(std::ostream &_out, SerializableMat &_v) {
        std::vector<uchar> buffer;
        cv::imencode(".jpg", _v.imgRef_, buffer, _v.params_);
        std::string_view res((char*)buffer.data(), buffer.size());
        _out << res;
        return _out;
    }
    friend std::istream & operator>>(std::istream &_in, SerializableMat &_v) {
        std::vector<uint8_t> buffer;
        std::for_each(std::istreambuf_iterator<char>(_in),
                    std::istreambuf_iterator<char>(),
                    [&buffer](const char c){
                        buffer.push_back(c);
                    });
        
        _v.imgRef_=cv::imdecode(buffer,1);
        return _in;
    }
    
private:
    std::vector<int> params_;
    cv::Mat imgRef_;
};


#endif