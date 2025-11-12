#include "Frame.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <algorithm>

Frame::Frame(const std::string& archivo)
    : archivo_(archivo) {
}

void Frame::setArchivo(const std::string& archivoNuevo) { archivo_ = archivoNuevo; }
const std::string& Frame::getArchivo() const { return archivo_; }

int  Frame::frameCount() const { return frameIndex_; }
bool Frame::isPlaying()  const { return playing_; }
int  Frame::getPlayIndex() const { return playIndex_; }

float Frame::getMovX() const { return movX_; }
float Frame::getMovY() const { return movY_; }
float Frame::getGiro() const { return giro_; }

void Frame::setMovX(float v) { movX_ = v; }
void Frame::setMovY(float v) { movY_ = v; }
void Frame::setGiro(float v) { giro_ = v; }

void Frame::resetToFirst() {
    if (frameIndex_ > 0) {
        movX_ = frames_[0].movX;
        movY_ = frames_[0].movY;
        giro_ = frames_[0].giro;
    }
}

void Frame::computeIncrementsFor(int idx, int i_max_steps) {
    if (idx + 1 >= frameIndex_) return;
    frames_[idx].movXInc = (frames_[idx + 1].movX - frames_[idx].movX) / i_max_steps;
    frames_[idx].movYInc = (frames_[idx + 1].movY - frames_[idx].movY) / i_max_steps;
    frames_[idx].giroInc = (frames_[idx + 1].giro - frames_[idx].giro) / i_max_steps;
}

void Frame::animateStep(int i_max_steps) {
    if (!playing_) return;

    if (i_curr_ >= i_max_steps) {
        playIndex_++;
        if (playIndex_ > frameIndex_ - 2) {
            std::printf("terminó la animación\n");
            playIndex_ = 0;
            playing_ = false;
        }
        else {
            i_curr_ = 0;
            computeIncrementsFor(playIndex_, i_max_steps);
        }
    }
    else {
        movX_ += frames_[playIndex_].movXInc;
        movY_ += frames_[playIndex_].movYInc;
        giro_ += frames_[playIndex_].giroInc;
        i_curr_++;
    }
}

void Frame::startPlayback(int i_max_steps) {
    if (frameIndex_ <= 1) {
        std::printf("No hay suficientes frames en %s para reproducir.\n", archivo_.c_str());
        return;
    }
    resetToFirst();
    playIndex_ = 0;
    i_curr_ = 0;
    computeIncrementsFor(playIndex_, i_max_steps);
    playing_ = true;
}

void Frame::stopPlayback() {
    playing_ = false;
    playIndex_ = 0;
    i_curr_ = 0;
}

void Frame::appendKeyToFile(const Key& frame, int index) {
    std::ofstream file(archivo_, std::ios::app);
    if (!file.is_open()) {
        std::printf("Error al abrir %s para escribir.\n", archivo_.c_str());
        return;
    }
    file << index << " " << frame.movX << " " << frame.movY << " " << frame.giro << "\n";
    std::printf("Frame %d agregado al archivo %s.\n", index, archivo_.c_str());
}

void Frame::saveCurrent(float movX, float movY, float giro) {
    if (frameIndex_ >= MAX_FRAMES) {
        std::printf("Límite de frames alcanzado (%d).\n", MAX_FRAMES);
        return;
    }
    frames_[frameIndex_].movX = movX;
    frames_[frameIndex_].movY = movY;
    frames_[frameIndex_].giro = giro;

    appendKeyToFile(frames_[frameIndex_], frameIndex_);
    frameIndex_++;
}

void Frame::loadFromDisk() {
    std::ifstream file(archivo_);
    if (!file.is_open()) {
        std::printf("No se encontró %s, se creará al guardar.\n", archivo_.c_str());
        return;
    }
    frameIndex_ = 0;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int idx; Key temp;
        if (iss >> idx >> temp.movX >> temp.movY >> temp.giro) {
            if (idx >= 0 && idx < MAX_FRAMES) {
                frames_[idx] = temp;
                frameIndex_ = std::max(frameIndex_, idx + 1);
            }
        }
    }
    std::printf("Se cargaron %d frames desde %s\n", frameIndex_, archivo_.c_str());
}

bool Frame::rewriteLine(int index, const Key& newFrame) {
    std::ifstream in(archivo_);
    if (!in.is_open()) {
        std::printf("No se puede abrir %s para modificar.\n", archivo_.c_str());
        return false;
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) lines.push_back(line);
    in.close();

    std::ofstream out(archivo_);
    if (!out.is_open()) {
        std::printf("No se puede escribir %s\n", archivo_.c_str());
        return false;
    }
    bool found = false;
    for (auto& l : lines) {
        std::istringstream iss(l);
        int idx; float x, y, g;
        if (iss >> idx >> x >> y >> g) {
            if (idx == index) {
                out << index << " " << newFrame.movX << " " << newFrame.movY << " " << newFrame.giro << "\n";
                found = true;
            }
            else {
                out << l << "\n";
            }
        }
    }
    return found;
}

bool Frame::eraseLine(int index) {
    std::ifstream in(archivo_);
    if (!in.is_open()) {
        std::printf("No se puede abrir %s para borrar.\n", archivo_.c_str());
        return false;
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) lines.push_back(line);
    in.close();

    std::ofstream out(archivo_);
    if (!out.is_open()) {
        std::printf("No se puede escribir %s\n", archivo_.c_str());
        return false;
    }
    bool found = false;
    for (auto& l : lines) {
        std::istringstream iss(l);
        int idx;
        if (iss >> idx) {
            if (idx == index) { found = true; continue; }
            out << l << "\n";
        }
    }
    return found;
}

bool Frame::modifyLast(float movX, float movY, float giro) {
    if (frameIndex_ <= 0) return false;
    int modIndex = frameIndex_ - 1;
    frames_[modIndex].movX = movX;
    frames_[modIndex].movY = movY;
    frames_[modIndex].giro = giro;

    bool ok = rewriteLine(modIndex, frames_[modIndex]);
    if (ok) std::printf("Frame %d modificado correctamente en %s.\n", modIndex, archivo_.c_str());
    else    std::printf("Frame %d no encontrado en %s.\n", modIndex, archivo_.c_str());
    return ok;
}

bool Frame::deleteLast() {
    if (frameIndex_ <= 0) return false;
    int delIndex = frameIndex_ - 1;
    bool ok = eraseLine(delIndex);
    if (ok) {
        frameIndex_--;
        std::printf("Frame %d eliminado correctamente de %s.\n", delIndex, archivo_.c_str());
    }
    else {
        std::printf("Frame %d no encontrado en %s.\n", delIndex, archivo_.c_str());
    }
    return ok;
}
