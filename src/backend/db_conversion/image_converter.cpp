////
//// Created by laurentiu on 13.06.2020.
////
//
//#include "image_converter.hpp"
//#include <qt5/QtCore/QCryptographicHash>
//
//ImageSsql ImageConverter::operator()(const BWImage& image, bool result) {
//    ImageSsql imageSsql;
//    imageSsql.id = 0L;
//    imageSsql.width = image.getWidth();
//    imageSsql.height = image.getHeight();
//    auto imageDataBytes = QByteArray::fromRawData(reinterpret_cast<const char*>(image.getData().data()),
//            image.getData().size());
//    imageSsql.checksum = QCryptographicHash::hash(imageDataBytes, QCryptographicHash::Algorithm::Sha1)
//            .toHex().
//            toStdString();
//    imageSsql.imageData = imageDataBytes.toStdString();
//    imageSsql.result = result;
//    return imageSsql;
//}
//
//ImageView ImageConverter::operator()(const ImageViewSsql& image) {
//    return ImageView{image.id, image.width, image.height, image.result};
//}
//
//std::vector<ImageView> ImageConverter::operator()(const std::vector<ImageViewSsql>& images) {
//    std::vector<ImageView> result;
//    for (auto& image : images) {
//        result.push_back((*this)(image));
//    }
//    return result;
//}
//
//BWImage ImageConverter::operator()(const ImageSsql& image) {
//    BWImage result(image.width, image.height);
//    auto iterator = image.imageData.begin();
//    for (auto& pixel : result.getData()) {
//        pixel = BWPixel(*iterator);
//        ++iterator;
//    }
//    return result;
//}
