// Copyright 2017 GeunYoung Lim <interruping4dev@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 @file geo_calc_distance.h
 @copyright CC BY-SA 3.0
 @see https://creativecommons.org/licenses/by-sa/3.0/
 @author https://stackoverflow.com/a/10205532/5766424
 */
 #ifdef __cplusplus
extern "C" {
#endif
#include <math.h>
    
#define earthRadiusKm 6371.0
    
    // This function converts decimal degrees to radians
    double deg2rad(double deg) {
        return (deg * M_PI / 180);
    }

    //  This function converts radians to decimal degrees
    double rad2deg(double rad) {
        return (rad * 180 / M_PI);
    }

    /**
     * Returns the distance between two points on the Earth.
     * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
     * @param lat1d Latitude of the first point in degrees
     * @param lon1d Longitude of the first point in degrees
     * @param lat2d Latitude of the second point in degrees
     * @param lon2d Longitude of the second point in degrees
     * @return The distance between the two points in kilometers
     */
    double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
        double lat1r, lon1r, lat2r, lon2r, u, v;
        lat1r = deg2rad(lat1d);
        lon1r = deg2rad(lon1d);
        lat2r = deg2rad(lat2d);
        lon2r = deg2rad(lon2d);
        u = sin((lat2r - lat1r)/2);
        v = sin((lon2r - lon1r)/2);
        return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
    }

#ifdef __cplusplus
}
#endif
