#pragma once
#include <string>
#include <vector>

class Frame {
public:
    struct Key {
        float movX = 0.0f;
        float movY = 0.0f;
        float movXInc = 0.0f;
        float movYInc = 0.0f;
        float giro = 0.0f;
        float giroInc = 0.0f;
    };

    // Construye un track de keyframes con archivo asociado (p. ej. "keyframes.txt")
    explicit Frame(const std::string& archivo = "keyframes.txt");

    // Archivo
    void setArchivo(const std::string& archivoNuevo);
    const std::string& getArchivo() const;

    // Persistencia
    void loadFromDisk();                                     // leerArchivo(...)
    void saveCurrent(float movX, float movY, float giro);    // saveFrame(...)
    bool modifyLast(float movX, float movY, float giro);     // actualizarArchivo(...)
    bool deleteLast();                                       // borrarFrame(...)

    // Reproducción
    void startPlayback(int i_max_steps); // resetElements + interpolation + play=true
    void stopPlayback();
    void animateStep(int i_max_steps);   // animate(...)

    // Estado de reproducción / consulta
    bool isPlaying() const;
    int  frameCount() const;
    int  getPlayIndex() const;

    // Valores actuales (los que usas al dibujar)
    float getMovX()   const;
    float getMovY()   const;
    float getGiro()   const;

    // Edición en vivo (H,K,U,J, flechas, etc.)
    void setMovX(float v);
    void setMovY(float v);
    void setGiro(float v);

private:
    static const int MAX_FRAMES = 100;

    std::string archivo_;
    Key frames_[MAX_FRAMES];
    int frameIndex_ = 0;      // cantidad de frames cargados/guardados
    int playIndex_ = 0;      // índice del tramo actual (entre frame i y i+1)
    int i_curr_ = 0;      // pasos acumulados dentro del tramo
    bool playing_ = false;  // está reproduciendo

    // Valores *actuales* que vas modificando al animar (antes vivían como variables sueltas)
    float movX_ = 0.0f;
    float movY_ = 0.0f;
    float giro_ = 0.0f;

    // Helpers (equivalen a resetElements / interpolation)
    void resetToFirst();
    void computeIncrementsFor(int idx, int i_max_steps);

    // Helpers de archivo (equivalen a appendKeyframeToFile / actualizarArchivo / borrarFrame)
    void appendKeyToFile(const Key& frame, int index);
    bool rewriteLine(int index, const Key& newFrame);
    bool eraseLine(int index);
};

