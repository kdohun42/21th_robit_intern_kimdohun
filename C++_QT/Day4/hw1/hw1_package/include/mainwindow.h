#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// UDP
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QHostAddress>

// 키보드
#include <QPushButton>
#include <QTimer>

#include <QFile>
#include <QTextStream>
#include <QStringList>

// 한/영 버튼 구현하기 위한 헤더
#include <QLabel>
#include <QHBoxLayout>

//#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    // ==================== UDP ====================

    // UDP 데이터가 들어오면 실행
    void udp_read();

    void on_pushButton_clicked();


    // ==================== 키보드 ====================

    void on_btnShift_toggled(bool checked);

    void on_btnABC_clicked();

    void on_btnDEF_clicked();

    void on_btnGHI_clicked();

    void on_btnJKL_clicked();

    void on_btnMNO_clicked();

    void on_btnPQRS_clicked();

    void on_btnTUV_clicked();

    void on_btnWXYZ_clicked();

    void confirmEnglishCharacter(); // 시간이 지나면 현재 글자 확정 함수

    void on_btnDelete_clicked(); // 글자 삭제 함수

    void on_btnEnter_clicked(); // 엔터 함수 파일로 입력한 텍스트 출력

    void on_btnSpace_clicked(); // 스페이스 함수

    void on_btnMark_clicked();

    void on_btnSpecialMark_clicked();

    //void on_btnKorean_clicked();

    //void on_btnEnglish_clicked();

    void on_btnLanguage_clicked();// 언어 변경

    void on_btnNumber_clicked();

    void on_btnComma_clicked();


private:

    Ui::MainWindow *ui;


    // ==================== UDP ====================

    // UDP Socket
    QUdpSocket *text_socket;

    // UDP 데이터 전송 함수
    void udp_write(
        const QByteArray &text,
        quint16 port,
        QUdpSocket &socket
        );


    // ==================== 키보드 ====================

    bool isUpperCase = false; // shift키 클릭 여부 확인 변수 false면 영어 소문자 true면 영어 대문자

    QPushButton *lastButton = nullptr;
    int currentIndex = 0;

    QTimer *englishTimer; // 영어 확정 타이머

    void shift_EnglishButtons(); // 영어 대문자로 변경하는 함수
    void inputEnglishKey(QPushButton *button, const QString &characters);

    bool Korean = false; // true 일 시 한글 모드로 변경
    bool Number = false; // true 일 시 숫자 모드로 변경

    QLabel *languageLabel; // 한/영 버튼
    void updateLanguageButton();


    // ==================== 한글 기능 ====================

    QString koreanInitial = ""; // 초성
    QString koreanVowel = ""; // 중성
    QString koreanFinal = ""; // 종성

    QString cheonjiinBuffer = ""; // 천지인 모음 입력 순서

    bool koreanComposing = false; // 현재 한글 한 글자 조합 중인지 확인

    QPushButton *koreanLastButton = nullptr; // 마지막으로 누른 자음 버튼
    int koreanIndex = 0; // 자음 버튼 내부 순환 위치

    QTimer *koreanTimer; // 자음 순환 시간 타이머

    void inputKoreanConsonant(QPushButton *button, const QString &characters); // 자음 입력
    void inputKoreanVowel(const QString &vowel); // 모음 입력

    QString makeCheonjiinVowel(const QString &buffer); // 천지인 모음 조합
    bool isCheonjiinPrefix(const QString &buffer); // 아직 만들 수 있는 모음인지 확인

    QString combineKorean(); // 초성 + 중성 + 종성 결합

    bool isValidFinalConsonant(const QString &consonant); // 받침 가능 여부

    QString combineFinalConsonant(const QString &first, const QString &second); // 겹받침 생성
    QStringList splitFinalConsonant(const QString &final); // 겹받침 분리

    void updateKoreanCharacter(); // 조합 중 글자 갱신
    void resetKoreanState(); // 한글 상태 초기화
};

#endif // MAINWINDOW_H
