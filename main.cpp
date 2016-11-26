#include <opencv2/opencv.hpp>
#include <dirent.h>

cv::Point top, bottom;
cv::Mat img, drawn;
bool down = false;

void CallBackFunc(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        top = cv::Point(x, y);
        down = true;
    } else if (down && event == cv::EVENT_MOUSEMOVE) {
        bottom = cv::Point(x, y);
        cv::rectangle(drawn, top, bottom, cv::Scalar(255, 0, 0), 3);
        cv::imshow("My Window", drawn);
        drawn = img.clone();
    } else if (event == cv::EVENT_LBUTTONUP) {
        down = false;
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
    return files;
}

int main(int argc, char** argv) {
    std::string dir = "/media/vladislav/HDD/Монетки/50 копеек/";
    std::string outdir = dir + "crop/";
    system(("mkdir \"" + outdir + "\"").c_str());
    auto files = getFiles(dir);

    for (const auto& f : files) {
        img = cv::imread(dir + f);

        if ( img.empty() ) {
            continue;
        }

        cv::resize(img, img, cv::Size(0,0), 0.3, 0.3);
        drawn = img.clone();

        cv::namedWindow("My Window", 1);
        cv::setMouseCallback("My Window", CallBackFunc, NULL);
        cv::imshow("My Window", drawn);

        while (true) {
            auto c = cv::waitKey();

            if (c == 536871027) {
                cv::Rect roi(top, bottom);
                auto crop = img(roi);
                cv::imwrite(outdir + f, crop);
                break;
            } else if (c == 536870944) {
                break;
            }
        }
    }

    return 0;
}
