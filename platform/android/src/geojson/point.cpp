#include "point.hpp"
#include "../java/util.hpp"

namespace mbgl {
namespace android {
namespace geojson {

mapbox::geojson::point Point::convert(jni::JNIEnv &env, jni::Object<Point> jPoint) {
    mapbox::geojson::point point;

    if (jPoint) {
        auto jDoubleList = Point::coordinates(env, jPoint);
        point = Point::convert(env, jDoubleList);
        jni::DeleteLocalRef(env, jDoubleList);
    }

    return point;
}

mapbox::geojson::point Point::convert(jni::JNIEnv &env, jni::Object<java::util::List/*<Double>*/> jDoubleList) {
        mapbox::geojson::point point;

        if (jDoubleList) {
            // TODO:
            jni::DeleteLocalRef(env, jDoubleList);
        }

        return point;
    }
jni::Object<java::util::List> Point::coordinates(jni::JNIEnv &env, jni::Object<Point> jPoint) {
     static auto method = Point::javaClass.GetMethod<jni::Object<java::util::List> ()>(env, "coordinates");
     return jPoint.Call(env, method);
}

void Point::registerNative(jni::JNIEnv &env) {
    // Lookup the class
    javaClass = *jni::Class<Point>::Find(env).NewGlobalRef(env).release();
}

jni::Class<Point> Point::javaClass;

} // namespace geojson
} // namespace android
} // namespace mbgl