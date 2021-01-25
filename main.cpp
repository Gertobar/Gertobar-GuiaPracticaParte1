#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <opencv2/core/core.hpp> // Define los objetos matemáticos que me permiten representar las imágenes
#include <opencv2/highgui/highgui.hpp> // Crear ventanas y manejar la interfaz gráfica de usuario
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio.hpp>


using namespace cv;
using namespace std;
Mat frame;
Mat gris;
Mat ruidoSal;
Mat ruidoPimienta;
Mat gauBlur;
Mat medBlur;
Mat canny;
Mat laplacian;

int porcentaje[2] = {0, 0};
int total[2] = {0, 0};
int px[2] = {0, 0};
int py[2] = {0, 0};


void trackBar(int v, void *p) {
    ruidoSal = gris.clone();

    total[0] = ((int) ((double) porcentaje[0] / 100 * ruidoSal.cols * ruidoSal.rows));
    while (total[0] > 0) {
        px[0] = (rand() % ruidoSal.cols);
        py[0] = (rand() % ruidoSal.rows);
        ruidoSal.at<uchar>(py[0], px[0]) = 255;
        total[0] -= 1;
    }
    imshow("sal", ruidoSal);
    total[0] = 0;
    px[0] = 0;
    py[0] = 0;

    GaussianBlur(ruidoSal, gauBlur, Size(3, 3), 15);
    imshow("blur", gauBlur);

    medianBlur(ruidoSal, medBlur, 1);
    imshow("mediana", medBlur);


    int threshold = 50;
    Canny(gauBlur, canny, threshold, threshold * 3, 3);
    imshow("canny", canny);

    Laplacian(ruidoSal, laplacian, CV_8UC1, 3);
    imshow("laplacian", laplacian);

}

void trackBarP(int v, void *p) {
    ruidoPimienta = gris.clone();

    total[1] = ((int) ((double) porcentaje[1] / 100 * gris.cols * gris.rows));
    while (total[1] > 0) {
        px[1] = (rand() % gris.cols);
        py[1] = (rand() % gris.rows);
        ruidoPimienta.at<uchar>(py[1], px[1]) = 0;
        total[1] -= 1;
    }
    imshow("pimienta", ruidoPimienta);
    total[1] = 0;
    px[1] = 0;
    py[1] = 0;

    GaussianBlur(ruidoPimienta, gauBlur, Size(3, 3), 15);
    imshow("blur", gauBlur);

    medianBlur(ruidoPimienta, medBlur, 3);
    imshow("mediana", medBlur);


    int threshold = 50;
    Canny(gauBlur, canny, threshold, threshold * 3, 3);
    imshow("canny", canny);

    Laplacian(ruidoPimienta, laplacian, CV_8UC1, 3);
    imshow("laplacian", laplacian);

}

int main() {

    VideoCapture video(0);
//    VideoCapture video(0);
    if (video.isOpened()) {
        namedWindow("video", WINDOW_AUTOSIZE);
        namedWindow("sal", WINDOW_AUTOSIZE);
        namedWindow("pimienta", WINDOW_AUTOSIZE);
        namedWindow("blur", WINDOW_AUTOSIZE);
        namedWindow("mediana", WINDOW_AUTOSIZE);
        namedWindow("canny", WINDOW_AUTOSIZE);
        namedWindow("laplacian", WINDOW_AUTOSIZE);

        createTrackbar("sal", "video", &porcentaje[0], 12, trackBar, NULL);
        createTrackbar("pimienta", "video", &porcentaje[1], 12, trackBarP, NULL);
        while (true) {
            video >> frame;
            if (!frame.empty()) {

                resize(frame, frame, Size(640, 480));
                cvtColor(frame, gris, COLOR_BGR2GRAY);
                total[0] = ((int) ((double) porcentaje[0] / 100 * ruidoSal.cols * ruidoSal.rows));
                while (total[0] > 0) {
                    px[0] = (rand() % ruidoSal.cols);
                    py[0] = (rand() % ruidoSal.rows);
                    ruidoSal.at<uchar>(py[0], px[0]) = 255;
                    total[0] -= 1;
                }
                total[1] = ((int) ((double) porcentaje[1] / 100 * gris.cols * gris.rows));
                while (total[1] > 0) {
                    px[1] = (rand() % gris.cols);
                    py[1] = (rand() % gris.rows);
                    ruidoPimienta.at<uchar>(py[1], px[1]) = 0;
                    total[1] -= 1;
                }
                imshow("video", gris);

                if (waitKey(23) == 27) {
                    break;
                }
            }
        }
    }
    destroyAllWindows();
    return 0;
}
