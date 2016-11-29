#include <opencv2/opencv.hpp>
#include <dirent.h>

static const int S = 536871027;
static const int SPACE = 536870944;

static cv::Point top, bottom;
static cv::Mat img, drawn;
static bool down = false;

void callback(int event, int x, int y, int, void*) {
    switch (event) {
        case cv::EVENT_LBUTTONDOWN:
            down = true;
            top = cv::Point(x, y);
            break;
        case cv::EVENT_LBUTTONUP:
            down = false;
            break;
        case cv::EVENT_MOUSEMOVE:
            if (down) {
                bottom = cv::Point(x, y);
                cv::rectangle(drawn, top, bottom, cv::Scalar(255, 0, 0), 3);
                cv::imshow("My Window", drawn);
                drawn = img.clone();
            }
            break;
        default:
            break;
    }
}

std::vector<std::string> getFiles(const std::string& dir) {
    std::vector<std::string> files;

    DIR *d;
    struct dirent *ent;
    if ((d = opendir(dir.c_str())) != NULL) {
        while ((ent = readdir(d)) != NULL) {
            files.push_back(std::string(ent->d_name));
        }
        closedir(d);
    } else {
        throw std::runtime_error("Wrong directory");
    }

    files.erase(files.begin(), files.begin() + 2);
    std::sort(files.begin(), files.end());
    return files;
}

int main(int argc, char** argv) {
    std::string dir = "/media/vladislav/HDD/coins/10 рублей/";
    std::string outdir = dir + "crop/";
    system(("mkdir \"" + outdir + "\"").c_str());
    auto files = getFiles(dir);

    cv::namedWindow("My Window", 1);
    cv::setMouseCallback("My Window", callback, NULL);

    for (const auto &f : files) {
        img = cv::imread(dir + f);

        if (img.empty()) continue;

        cv::resize(img, img, cv::Size(0, 0), 0.3, 0.3);
        drawn = img.clone();
        cv::rectangle(drawn, top, bottom, cv::Scalar(255, 0, 0), 3);

        cv::imshow("My Window", drawn);

        while (true) {
            auto c = cv::waitKey();

            if (c == S) {
                cv::Rect roi(top, bottom);
                if (roi.width > 0 && roi.height > 0) {
                    auto crop = img(roi);
                    cv::imwrite(outdir + f, crop);
                    break;
                }
            } else if (c == SPACE) {
                break;
            }
        }
    }

    return 0;
}
