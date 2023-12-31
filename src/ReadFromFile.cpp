#include "ReadFromFile.h"
#include "Box.h"

static HWND hwnd;
static TCHAR szFileName[MAX_PATH];

ReadFromFile::ReadFromFile(float _x, float _y, float _width, float _height,
    float _xWarning, float _yWarning, float _widthWarning, float _heightWarning,
    sf::Font* _font, int _maxSize, int _minValue, int _maxValue, InputType _type) :
    x(_x), y(_y), width(_width), height(_height), 
    xWarning(_xWarning), yWarning(_yWarning), widthWarning(_widthWarning), heightWarning(_heightWarning),
    font(_font), maxSize(_maxSize), minValue(_minValue), maxValue(_maxValue),
    isDisplaying(false), displayingWarning(false), fileName("Choose file"), type(_type) {}

std::wstring charPtrToWideString(const char* charString) {
    // Create a std::wstring_convert using std::codecvt_utf8
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

    // Use the converter to convert the char* to std::wstring
    std::wstring wideString = converter.from_bytes(charString);

    return wideString;
}

void ReadFromFile::handleMousePressed(float mouseX, float mouseY) {
    if (mouseX < x || mouseX > x + width || mouseY < y || mouseY > y + height || !isDisplaying) {
        return;
    }
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = "txt";
    if (GetOpenFileName(&ofn) == TRUE) {
        std::wstring filePath = charPtrToWideString(ofn.lpstrFile);
        std::size_t lastSlashPos = filePath.find_last_of('\\');
        std::wstring wfileName = filePath.substr(lastSlashPos + 1);
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string fileNameUtf8 = converter.to_bytes(wfileName);
        fileName = fileNameUtf8;
        std::cout << fileName << "\n";

        std::wstring filePath2 = charPtrToWideString(ofn.lpstrFile);
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter2;
        address = converter2.to_bytes(filePath2);
        std::cout << address << "\n";
        setWarning();
    }
    else {
        // User cancelled the dialog
    }
    ofn.lpstrFile = NULL;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrFilter = NULL;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxFile = 0;
    ofn.nMaxCustFilter = 0;
    ofn.lpstrTitle = NULL;
    ofn.lpstrInitialDir = NULL;
}

void ReadFromFile::handleMouseMove(float mouseX, float mouseY, sf::RenderWindow& window) {
    if (mouseX < x || mouseX > x + width || mouseY < y || mouseY > y + height || !isDisplaying) {
        return;
    }
    window.setMouseCursor(handCursor);
}

void ReadFromFile::draw(sf::RenderWindow& window, ColorTheme theme) {
    if (fileName != "Choose file") {
        setWarning();
    }
    if (isDisplaying) {
        Box box(x, y, width, height, { ColorBoxType::UploadBox }, fileName, font, 20, true, 1);
        box.draw(window, theme);
        if (displayingWarning) {
            sf::RectangleShape tmpRect(sf::Vector2f(widthWarning, heightWarning));
            tmpRect.setPosition(xWarning, yWarning);
            tmpRect.setFillColor(warningFillColor[theme]);
            window.draw(tmpRect);
            sf::Text WarningText = CompressWords(warning,
                xWarning, yWarning, widthWarning, heightWarning,
                font, 20, warningTextColor[theme]);
            window.draw(WarningText);
        }
    }
}

std::vector <int> ReadFromFile::getListInt() {
    if (fileName == "Choose file") {
        return {};
    }
    std::ifstream fi(address);
    std::vector <int> tmp;
    while (!fi.eof()) {
        std::string line;
        std::getline(fi, line);
        int i = 0;
        std::string valText;
        std::string curText = line + ",";
        while (i < curText.size()) {
            if (curText[i] == ',' || curText[i] == ' ') {
                if (!valText.empty()) {
                    tmp.push_back(stringToInt(valText));
                    valText.clear();
                    if (tmp.size() > maxSize) {
                        return { -4 };
                    }
                }
                i++;
                continue;
            }
            if (curText[i] < '0' || curText[i] > '9') {
                fi.close();
                return { -1 };
            }
            valText.push_back(curText[i]);
            if (valText.size() > 9) {
                fi.close();
                return { -3 };
            }
            if (stringToInt(valText) < minValue || stringToInt(valText) > maxValue) {
                fi.close();
                return { -2 };
            }
            i++;
        }
    }
    fi.close();
    return tmp;
}

std::pair <std::vector <GeneralEdge>, int> ReadFromFile::getListEdge() {
    if (fileName == "Choose file") {
        return {};
    }
    std::ifstream fi(address);
    //first number = size;
    std::string firstLine;
    std::getline(fi, firstLine);
    int size = 0;
    for (char x : firstLine) {
        if (x < '0' || x > '9') {
            fi.close();
            return { {}, -1 };
        }
        size = size * 10 + x - '0';
    }
    if (size > maxSize) {
        fi.close();
        return { {}, -4 };
    }
    std::vector <GeneralEdge> tmp;
    for (int numRow = 1; numRow <= size; numRow++) {
        std::string line;
        std::getline(fi, line);
        int i = 0, numCol = 1;
        std::string valText;
        std::string curText = line + " ";
        while (i < curText.size()) {
            if (curText[i] == ' ') {
                if (!valText.empty()) {
                    int val = stringToInt(valText);
                    tmp.push_back({ numRow, numCol, stringToInt(valText) });
                    valText.clear();
                    if (numCol > size) {
                        fi.close();
                        return { {}, -4 };
                    }
                    numCol++;
                }
                i++;
                continue;
            }
            if (curText[i] < '0' || curText[i] > '9') {
                fi.close();
                return { {}, -1 };
            }
            valText.push_back(curText[i]);
            if (valText.size() > 9) {
                fi.close();
                return { {}, -3 };
            }
            if (stringToInt(valText) < minValue || stringToInt(valText) > maxValue) {
                fi.close();
                return { {}, -2 };
            }
            i++;
        }
        if (numCol - 1 != size) {
            fi.close();
            return { {}, -5 };
        }
    }
    fi.close();
    while (true) {
        bool found = false;
        for (int i = 0; i < tmp.size(); i++) {
            if (tmp[i].from == tmp[i].to) {
                tmp.erase(tmp.begin() + i);
                found = true;
                break;
            }
        }
        if (!found) {
            break;
        }
    }
    return {tmp, size};
}

std::pair <std::vector <std::string>, int> ReadFromFile::getListString() {
    if (fileName == "Choose file") {
        return {{}, 0};
    }
    std::ifstream fi(address);
    //first number = size;
    std::vector <std::string> tmp;
    while (!fi.eof()) {
        std::string line;
        std::getline(fi, line);
        int i = 0;
        std::string valText;
        std::string curText = line + ",";
        while (i < curText.size()) {
            if (curText[i] == ',' || curText[i] == ' ') {
                if (!valText.empty()) {
                    tmp.push_back(valText);
                    if (tmp.size() > maxSizeDataTrie) {
                        fi.close();
                        return {{}, -2 };
                    }
                    valText.clear();
                }
                i++;
                continue;
            }
            if (curText[i] < 'a' || curText[i] > 'z') {
                fi.close();
                return {{}, -1 };
            }
            valText.push_back(curText[i]);
            if (valText.size() > maxLengthDataTrie) {
                fi.close();
                return {{}, -3 };
            }
            i++;
        }
    }
    fi.close();
    return {tmp, 0};
}

void ReadFromFile::setWarning() {
    if (type == MatrixGraphInput) {
        std::pair <std::vector <GeneralEdge>, int> tmp = getListEdge();
        if (tmp.second == -1) {
            displayingWarning = true;
            warning = "Illegar character found, character must be space or number (0..9)!";
            return;
        }
        if (tmp.second == -2 || tmp.second == -3) {
            displayingWarning = true;
            warning = "Value must be in [";
            warning = warning + intToString(minValue) + "," + intToString(maxValue) + "]!";
            return;
        }
        if (tmp.second == -4) {
            displayingWarning = true;
            warning = "Too many values on one line!";
            return;
        }
        if (tmp.second == -5) {
            displayingWarning = true;
            warning = "Matrix is missing numbers!";
            return;
        }
        if (!tmp.first.empty()) {
            displayingWarning = false;
        }
        return;
    }
    if (type == StringListInput) {
        std::pair <std::vector <std::string>, int> tmp = getListString();
        if (tmp.second == -1) {
            displayingWarning = true;
            warning = "Illegar character found, character must be space, comma, or letter (a..z)!";
            return;
        }
        if (tmp.second == -2) {
            displayingWarning = true;
            warning = "Maximum number of string is " + intToString(maxSizeDataTrie) + "!";
            return;
        }
        if (tmp.second == -3) {
            displayingWarning = true;
            warning = "A single string's length must NOT be greater than " + intToString(maxLengthDataTrie) + "!";
            return;
        }
        if (!tmp.first.empty()) {
            displayingWarning = false;
        }
        return;
    }
    std::vector <int> tmp = getListInt();
    if (!tmp.empty() && tmp[0] == -1) {
        displayingWarning = true;
        warning = "Illegar character found, character must be space, comma, or number (0..9)!";
        return;
    }
    if (!tmp.empty() && (tmp[0] == -2 || tmp[0] == -3)) {
        displayingWarning = true;
        warning = "Value must be in [";
        warning = warning + intToString(minValue) + "," + intToString(maxValue) + "]!";
        return;
    }
    if (!tmp.empty() && (tmp[0] == -4)) {
        displayingWarning = true;
        warning = "Maximum size is " + intToString(maxSize);
        return;
    }
    if (!tmp.empty() && type == HashTableInput && tmp.size() - 1 > tmp[0]) {
        displayingWarning = true;
        warning = "Number of elements must NOT be greater than " + intToString(tmp[0]);
        return;
    }
    displayingWarning = false;
}

void ReadFromFile::setDisplaying(bool val) {
    isDisplaying = val;
}

bool ReadFromFile::getDisplaying() {
    return isDisplaying;
}

void ReadFromFile::reset() {
    isDisplaying = false;
    displayingWarning = false;
    fileName = "Choose file";
    address = address = "";
}