#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    languageLabel = new QLabel(ui->btnLanguage);

    languageLabel->setAlignment(Qt::AlignCenter);
    languageLabel->setTextFormat(Qt::RichText);
    languageLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    QHBoxLayout *languageLayout = new QHBoxLayout(ui->btnLanguage);
    languageLayout->setContentsMargins(0, 0, 0, 0);
    languageLayout->addWidget(languageLabel);

    ui->btnLanguage->setText("");

    updateLanguageButton();

    englishTimer = new QTimer(this);

    englishTimer->setSingleShot(true); // 한 번만 실행
    englishTimer->setInterval(500); // 글자 확정 시간 0.5초

    connect(englishTimer, &QTimer::timeout, this, &MainWindow::confirmEnglishCharacter);


    // 한글 자음 순환 타이머
    koreanTimer = new QTimer(this);

    koreanTimer->setSingleShot(true);
    koreanTimer->setInterval(1000);

    connect(koreanTimer, &QTimer::timeout, this, [this]()
            {
                koreanLastButton = nullptr;
                koreanIndex = 0;
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::shift_EnglishButtons()
{
    ui->btnShift-> setText(isUpperCase ? "⬆" : "⇧");
    ui->btnABC->setText(isUpperCase ? "ABC" : "abc");
    ui->btnDEF->setText(isUpperCase ? "DEF" : "def");
    ui->btnGHI->setText(isUpperCase ? "GHI" : "ghi");
    ui->btnJKL->setText(isUpperCase ? "JKL" : "jkl");
    ui->btnMNO->setText(isUpperCase ? "MNO" : "mno");
    ui->btnPQRS->setText(isUpperCase ? "PQRS" : "pqrs");
    ui->btnTUV->setText(isUpperCase ? "TUV" : "tuv");
    ui->btnWXYZ->setText(isUpperCase ? "WXYZ" : "wxyz");
}

void MainWindow::on_btnShift_toggled(bool checked)
{
    if(Korean)
    {
        // 한글 조합 확정 후 특수문자 입력
        resetKoreanState();
        inputEnglishKey(ui->btnShift, ".,?!");
    }

    else if(Number)
        inputEnglishKey(ui->btnShift, ".,?!");

    else
    {
        //qDebug() << "shift_English" << checked;
        //ui->btnShift-> setText("⬆");
        isUpperCase = checked;
        shift_EnglishButtons();
    }
}


// 함수명은 inputEnglishKey지만 영어, 숫자, 특수문자 출력할 때 사용
void MainWindow::inputEnglishKey(QPushButton *button, const QString &characters)
{
    englishTimer->start();

    if (lastButton == button)
    {
        currentIndex++;

        if (currentIndex >= characters.length())
            currentIndex = 0;

        ui->lineEdit->backspace();
    }
    else
    {
        lastButton = button;
        currentIndex = 0;
    }

    QString text(characters[currentIndex]);

    if (isUpperCase)
        text = text.toUpper();

    ui->lineEdit->insert(text);
}

void MainWindow::on_btnSpecialMark_clicked()
{
    if (Korean)
        inputKoreanVowel("ㅣ");

    else if(Number)
        inputEnglishKey(ui->btnSpecialMark, "1(-)");

    else
        inputEnglishKey(ui->btnSpecialMark, ".,?!");
}


void MainWindow::on_btnABC_clicked()
{
    if (Korean)
        inputKoreanVowel("ㆍ");

    else if(Number)
        inputEnglishKey(ui->btnABC, "2$");

    else
        inputEnglishKey(ui->btnABC, "abc");
}


void MainWindow::on_btnDEF_clicked()
{
    if (Korean)
        inputKoreanVowel("ㅡ");

    else if(Number)
        inputEnglishKey(ui->btnDEF, "3^%#");

    else
        inputEnglishKey(ui->btnDEF, "def");
}


void MainWindow::on_btnGHI_clicked()
{
    if (Korean)
        inputKoreanConsonant(ui->btnGHI, "ㄱㅋㄲ");

    else if(Number)
        inputEnglishKey(ui->btnGHI, "4{*}");

    else
        inputEnglishKey(ui->btnGHI, "ghi");
}


void MainWindow::on_btnJKL_clicked()
{
    if (Korean)
        inputKoreanConsonant(ui->btnJKL, "ㄴㄹ");

    else if(Number)
        inputEnglishKey(ui->btnJKL, "5+×÷");

    else
        inputEnglishKey(ui->btnJKL, "jkl");
}


void MainWindow::on_btnMNO_clicked()
{
    if (Korean)
        inputKoreanConsonant(ui->btnMNO, "ㄷㅌㄸ");

    else if(Number)
        inputEnglishKey(ui->btnMNO, "6<=>");

    else
        inputEnglishKey(ui->btnMNO, "mno");
}


void MainWindow::on_btnPQRS_clicked()
{
    if (Korean)
        inputKoreanConsonant(ui->btnPQRS, "ㅂㅍㅃ");

    else if(Number)
        inputEnglishKey(ui->btnPQRS, "7[:]");

    else
        inputEnglishKey(ui->btnPQRS, "pqrs");
}


void MainWindow::on_btnTUV_clicked()
{
    if (Korean)
        inputKoreanConsonant(ui->btnTUV, "ㅅㅎㅆ");

    else if(Number)
        inputEnglishKey(ui->btnTUV, "8\"';");

    else
        inputEnglishKey(ui->btnTUV, "tuv");
}


void MainWindow::on_btnWXYZ_clicked()
{
    if (Korean)
        inputKoreanConsonant(ui->btnWXYZ, "ㅈㅊㅉ");

    else if(Number)
        inputEnglishKey(ui->btnWXYZ, "9/\\|");

    else
        inputEnglishKey(ui->btnWXYZ, "wxyz");
}


void MainWindow::on_btnMark_clicked()
{
    if (Korean)
        inputKoreanConsonant(ui->btnMark, "ㅇㅁ");
    else if(Number)
        inputEnglishKey(ui->btnMark, "0~.");
    /* 클릭은 되지만 아무 것도 하지 않음
    else
        ui->btnMark->setEnabled(false);
    */
}


//  영어 확정

void MainWindow::confirmEnglishCharacter()
{
    lastButton = nullptr;
    currentIndex = 0;
}

void MainWindow::on_btnDelete_clicked()
{
    ui->lineEdit->backspace(); // 커서 앞 글자 삭제

    lastButton = nullptr;
    currentIndex = 0;
    englishTimer->stop();

    if (Korean)
        resetKoreanState();
}

void MainWindow::on_btnEnter_clicked()
{
    QString text = ui->lineEdit->text();

    if (text.isEmpty())
        return;

    QFile file("/home/kdh/Desktop/21th_robit_intern_kimdohun/C++_QT/Day3/hw3/message.txt");

    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << text << "\n";
        file.close();

        ui->lineEdit->clear();

        lastButton = nullptr;
        currentIndex = 0;
        englishTimer->stop();

        resetKoreanState();
    }
}

void MainWindow::on_btnSpace_clicked()
{
    if (Korean)
        resetKoreanState();

    ui->lineEdit->insert(" "); // 공백 입력

    lastButton = nullptr;
    currentIndex = 0;
    englishTimer->stop();
}

//한글 기능

// 받침으로 사용할 수 있는 자음인지 확인
bool MainWindow::isValidFinalConsonant(const QString &consonant)
{
    static const QStringList finals =
        {
            "ㄱ", "ㄲ", "ㄳ",
            "ㄴ", "ㄵ", "ㄶ",
            "ㄷ",
            "ㄹ", "ㄺ", "ㄻ", "ㄼ", "ㄽ", "ㄾ", "ㄿ", "ㅀ",
            "ㅁ",
            "ㅂ", "ㅄ",
            "ㅅ", "ㅆ",
            "ㅇ",
            "ㅈ", "ㅊ",
            "ㅋ", "ㅌ", "ㅍ", "ㅎ"
        };

    return finals.contains(consonant);
}


// 겹받침 생성
QString MainWindow::combineFinalConsonant(const QString &first, const QString &second)
{
    if (first == "ㄱ" && second == "ㅅ") return "ㄳ";

    if (first == "ㄴ" && second == "ㅈ") return "ㄵ";
    if (first == "ㄴ" && second == "ㅎ") return "ㄶ";

    if (first == "ㄹ" && second == "ㄱ") return "ㄺ";
    if (first == "ㄹ" && second == "ㅁ") return "ㄻ";
    if (first == "ㄹ" && second == "ㅂ") return "ㄼ";
    if (first == "ㄹ" && second == "ㅅ") return "ㄽ";
    if (first == "ㄹ" && second == "ㅌ") return "ㄾ";
    if (first == "ㄹ" && second == "ㅍ") return "ㄿ";
    if (first == "ㄹ" && second == "ㅎ") return "ㅀ";

    if (first == "ㅂ" && second == "ㅅ") return "ㅄ";

    return "";
}


// 겹받침 분리
QStringList MainWindow::splitFinalConsonant(const QString &final)
{
    if (final == "ㄳ") return {"ㄱ", "ㅅ"};

    if (final == "ㄵ") return {"ㄴ", "ㅈ"};
    if (final == "ㄶ") return {"ㄴ", "ㅎ"};

    if (final == "ㄺ") return {"ㄹ", "ㄱ"};
    if (final == "ㄻ") return {"ㄹ", "ㅁ"};
    if (final == "ㄼ") return {"ㄹ", "ㅂ"};
    if (final == "ㄽ") return {"ㄹ", "ㅅ"};
    if (final == "ㄾ") return {"ㄹ", "ㅌ"};
    if (final == "ㄿ") return {"ㄹ", "ㅍ"};
    if (final == "ㅀ") return {"ㄹ", "ㅎ"};

    if (final == "ㅄ") return {"ㅂ", "ㅅ"};

    return {};
}


// 한글 자음 입력
void MainWindow::inputKoreanConsonant(QPushButton *button, const QString &characters)
{
    bool sameButton = koreanLastButton == button;

    if (sameButton)
    {
        koreanIndex++;

        if (koreanIndex >= characters.length())
            koreanIndex = 0;
    }
    else
    {
        koreanLastButton = button;
        koreanIndex = 0;
    }

    koreanTimer->start();

    QString consonant = QString(characters[koreanIndex]);


    // 모음만 입력되어 있는 경우 기존 모음 확정 후 자음 시작
    if (koreanInitial.isEmpty() && !koreanVowel.isEmpty())
    {
        koreanComposing = false;

        koreanVowel.clear();
        koreanFinal.clear();
        cheonjiinBuffer.clear();

        koreanInitial = consonant;

        updateKoreanCharacter();

        return;
    }


    // 아무것도 없는 상태
    if (koreanInitial.isEmpty())
    {
        koreanInitial = consonant;

        updateKoreanCharacter();

        return;
    }


    // 초성만 있는 상태
    if (koreanVowel.isEmpty())
    {
        // 같은 버튼을 빠르게 누르면 자음 변경
        if (sameButton)
        {
            koreanInitial = consonant;

            updateKoreanCharacter();

            return;
        }


        // 다른 자음이면 기존 자음 확정 후 새로운 자음 시작
        koreanComposing = false;

        koreanInitial = consonant;
        koreanVowel.clear();
        koreanFinal.clear();
        cheonjiinBuffer.clear();

        updateKoreanCharacter();

        return;
    }


    // 초성 + 중성만 존재하는 경우
    if (koreanFinal.isEmpty())
    {
        // ㄸ, ㅃ, ㅉ는 받침으로 사용 불가능
        if (!isValidFinalConsonant(consonant))
        {
            koreanComposing = false;

            koreanInitial = consonant;
            koreanVowel.clear();
            koreanFinal.clear();
            cheonjiinBuffer.clear();

            updateKoreanCharacter();

            return;
        }

        koreanFinal = consonant;

        updateKoreanCharacter();

        return;
    }


    // 이미 겹받침인 경우
    QStringList currentSplit = splitFinalConsonant(koreanFinal);

    if (!currentSplit.isEmpty())
    {
        // 같은 버튼이면 겹받침의 뒤 자음을 변경할 수 있는지 확인
        if (sameButton)
        {
            QString firstFinal = currentSplit[0];

            // 현재 버튼의 후보 중 가능한 겹받침 검색
            for (int i = 0; i < characters.length(); i++)
            {
                int index = (koreanIndex + i) % characters.length();

                QString candidate = QString(characters[index]);
                QString combined = combineFinalConsonant(firstFinal, candidate);

                if (!combined.isEmpty())
                {
                    koreanIndex = index;
                    koreanFinal = combined;

                    updateKoreanCharacter();

                    return;
                }
            }
        }


        // 기존 글자 확정 후 새로운 초성 시작
        koreanComposing = false;

        koreanInitial = consonant;
        koreanVowel.clear();
        koreanFinal.clear();
        cheonjiinBuffer.clear();

        updateKoreanCharacter();

        return;
    }


    // 같은 버튼을 눌러 현재 받침 변경
    if (sameButton)
    {
        // ㄷ->ㅌ->ㄸ 중 ㄸ는 받침 불가이므로 건너뜀
        for (int i = 0; i < characters.length(); i++)
        {
            int index = (koreanIndex + i) % characters.length();

            QString candidate = QString(characters[index]);

            if (isValidFinalConsonant(candidate))
            {
                koreanIndex = index;
                koreanFinal = candidate;

                updateKoreanCharacter();

                return;
            }
        }
    }


    // 현재 종성과 새 자음으로 겹받침 생성 시도
    QString combinedFinal = combineFinalConsonant(koreanFinal, consonant);

    if (!combinedFinal.isEmpty())
    {
        koreanFinal = combinedFinal;

        updateKoreanCharacter();

        return;
    }


    // 첫 번째 자음으로 겹받침이 안 만들어지는 경우
    // 같은 버튼의 다른 자음도 확인
    for (int i = 0; i < characters.length(); i++)
    {
        QString candidate = QString(characters[i]);

        combinedFinal = combineFinalConsonant(koreanFinal, candidate);

        if (!combinedFinal.isEmpty())
        {
            koreanIndex = i;
            koreanFinal = combinedFinal;

            updateKoreanCharacter();

            return;
        }
    }


    // 겹받침을 만들 수 없으면 현재 글자 확정
    koreanComposing = false;

    koreanInitial = consonant;
    koreanVowel.clear();
    koreanFinal.clear();
    cheonjiinBuffer.clear();

    updateKoreanCharacter();
}


// 현재 입력이 천지인 모음의 일부인지 확인
bool MainWindow::isCheonjiinPrefix(const QString &buffer)
{
    static const QStringList sequences =
        {
            // ㅣ
            "ㅣ",

            // ㅡ
            "ㅡ",

            // ㅏ ㅐ ㅑ ㅒ
            "ㅣㆍ",
            "ㅣㆍㅣ",
            "ㅣㆍㆍ",
            "ㅣㆍㆍㅣ",

            // ㅓ ㅔ ㅕ ㅖ
            "ㆍㅣ",
            "ㆍㅣㅣ",
            "ㆍㆍㅣ",
            "ㆍㆍㅣㅣ",

            // ㅗ ㅚ ㅘ ㅙ
            "ㆍㅡ",
            "ㆍㅡㅣ",
            "ㆍㅡㅣㆍ",
            "ㆍㅡㅣㆍㅣ",

            // ㅛ
            "ㆍㆍㅡ",

            // ㅜ ㅟ ㅝ ㅞ
            "ㅡㆍ",
            "ㅡㆍㅣ",
            "ㅡㆍㆍㅣ",
            "ㅡㆍㆍㅣㅣ",

            // ㅠ
            "ㅡㆍㆍ",

            // ㅢ
            "ㅡㅣ"
        };

    // ㆍ 하나도 모음 조합 중간 상태
    if (buffer == "ㆍ" || buffer == "ㆍㆍ")
        return true;

    for (const QString &sequence : sequences)
    {
        if (sequence.startsWith(buffer))
            return true;
    }

    return false;
}


// 천지인 입력을 실제 중성으로 변환
QString MainWindow::makeCheonjiinVowel(const QString &buffer)
{
    // 기본 모음
    if (buffer == "ㅣ") return "ㅣ";
    if (buffer == "ㅡ") return "ㅡ";
    // ㅏ
    if (buffer == "ㅣㆍ") return "ㅏ";
    // ㅐ
    if (buffer == "ㅣㆍㅣ") return "ㅐ";
    // ㅑ
    if (buffer == "ㅣㆍㆍ") return "ㅑ";
    // ㅒ
    if (buffer == "ㅣㆍㆍㅣ") return "ㅒ";
    // ㅓ
    if (buffer == "ㆍㅣ") return "ㅓ";
    // ㅔ
    if (buffer == "ㆍㅣㅣ") return "ㅔ";
    // ㅕ
    if (buffer == "ㆍㆍㅣ") return "ㅕ";
    // ㅖ
    if (buffer == "ㆍㆍㅣㅣ") return "ㅖ";
    // ㅗ
    if (buffer == "ㆍㅡ") return "ㅗ";
    // ㅚ
    if (buffer == "ㆍㅡㅣ") return "ㅚ";
    // ㅘ
    if (buffer == "ㆍㅡㅣㆍ") return "ㅘ";
    // ㅙ
    if (buffer == "ㆍㅡㅣㆍㅣ") return "ㅙ";
    // ㅛ
    if (buffer == "ㆍㆍㅡ") return "ㅛ";
    // ㅜ
    if (buffer == "ㅡㆍ") return "ㅜ";
    // ㅟ
    if (buffer == "ㅡㆍㅣ") return "ㅟ";
    // ㅝ
    if (buffer == "ㅡㆍㆍㅣ") return "ㅝ";
    // ㅞ
    if (buffer == "ㅡㆍㆍㅣㅣ") return "ㅞ";
    // ㅠ
    if (buffer == "ㅡㆍㆍ") return "ㅠ";
    // ㅢ
    if (buffer == "ㅡㅣ") return "ㅢ";

    return "";
}


// 한글 모음 입력
void MainWindow::inputKoreanVowel(const QString &vowel)
{
    koreanLastButton = nullptr;
    koreanIndex = 0;
    koreanTimer->stop();


    // 종성이 있는 상태에서 모음을 누른 경우
    if (!koreanFinal.isEmpty())
    {
        QString nextInitial;

        QStringList split = splitFinalConsonant(koreanFinal);


        // 겹받침인 경우
        if (!split.isEmpty())
        {
            // 앞 자음은 기존 글자의 받침으로 남김
            koreanFinal = split[0];

            // 뒤 자음은 다음 글자의 초성
            nextInitial = split[1];
        }
        else
        {
            // 일반 받침은 전체를 다음 초성으로 이동
            nextInitial = koreanFinal;
            koreanFinal.clear();
        }


        // 기존 글자에서 받침 제거 또는 겹받침 수정
        updateKoreanCharacter();


        // 기존 글자 확정
        koreanComposing = false;


        // 다음 글자 시작
        koreanInitial = nextInitial;
        koreanVowel.clear();
        koreanFinal.clear();
        cheonjiinBuffer.clear();
    }


    QString nextBuffer = cheonjiinBuffer + vowel;


    // 현재 모음에 더 이상 결합할 수 없는 경우
    if (!isCheonjiinPrefix(nextBuffer))
    {
        // 기존 글자 확정
        koreanComposing = false;

        // 새로운 모음 시작
        koreanInitial.clear();
        koreanVowel.clear();
        koreanFinal.clear();

        cheonjiinBuffer = vowel;
    }
    else
    {
        cheonjiinBuffer = nextBuffer;
    }


    QString result = makeCheonjiinVowel(cheonjiinBuffer);


    // 완성 가능한 모음이면 저장
    if (!result.isEmpty())
        koreanVowel = result;


    updateKoreanCharacter();
}


// 초성 + 중성 + 종성을 한글 한 글자로 변환
QString MainWindow::combineKorean()
{
    static const QStringList initials =
        {
            "ㄱ", "ㄲ", "ㄴ", "ㄷ", "ㄸ",
            "ㄹ", "ㅁ", "ㅂ", "ㅃ", "ㅅ",
            "ㅆ", "ㅇ", "ㅈ", "ㅉ", "ㅊ",
            "ㅋ", "ㅌ", "ㅍ", "ㅎ"
        };


    static const QStringList vowels =
        {
            "ㅏ", "ㅐ", "ㅑ", "ㅒ",
            "ㅓ", "ㅔ", "ㅕ", "ㅖ",
            "ㅗ", "ㅘ", "ㅙ", "ㅚ",
            "ㅛ",
            "ㅜ", "ㅝ", "ㅞ", "ㅟ",
            "ㅠ",
            "ㅡ", "ㅢ", "ㅣ"
        };


    static const QStringList finals =
        {
            "",
            "ㄱ", "ㄲ", "ㄳ",
            "ㄴ", "ㄵ", "ㄶ",
            "ㄷ",
            "ㄹ", "ㄺ", "ㄻ", "ㄼ", "ㄽ", "ㄾ", "ㄿ", "ㅀ",
            "ㅁ",
            "ㅂ", "ㅄ",
            "ㅅ", "ㅆ",
            "ㅇ",
            "ㅈ", "ㅊ",
            "ㅋ", "ㅌ", "ㅍ", "ㅎ"
        };


    // 초성이 없이 모음만 입력한 경우
    if (koreanInitial.isEmpty())
        return koreanVowel;


    // 초성만 입력한 경우
    if (koreanVowel.isEmpty())
        return koreanInitial;


    int initialIndex = initials.indexOf(koreanInitial);
    int vowelIndex = vowels.indexOf(koreanVowel);
    int finalIndex = finals.indexOf(koreanFinal);


    if (initialIndex == -1 || vowelIndex == -1)
        return koreanInitial;


    if (finalIndex == -1)
        finalIndex = 0;


    // 한글 유니코드 조합 공식
    int unicode = 0xAC00 + (initialIndex * 21 + vowelIndex) * 28 + finalIndex;


    return QString(QChar(unicode));
}


// 현재 조합 중인 한글 갱신
void MainWindow::updateKoreanCharacter()
{
    QString result = combineKorean();


    // 아직 완성된 출력 결과가 없는 경우
    if (result.isEmpty())
        return;


    // 기존 조합 중 글자를 삭제
    if (koreanComposing)
        ui->lineEdit->backspace();


    // 새 글자 삽입
    ui->lineEdit->insert(result);


    koreanComposing = true;
}


// 한글 상태 초기화
void MainWindow::resetKoreanState()
{
    koreanInitial.clear();
    koreanVowel.clear();
    koreanFinal.clear();

    cheonjiinBuffer.clear();

    koreanComposing = false;

    koreanLastButton = nullptr;
    koreanIndex = 0;

    koreanTimer->stop();
}
/*
void MainWindow::on_btnKorean_clicked()
{
    // 둘 다 true 되는 것을 방지
    Korean = true;
    Number = false;

    lastButton = nullptr;
    currentIndex = 0;
    englishTimer->stop();

    resetKoreanState();

    ui->btnSpecialMark->setText("ㅣ");
    ui->btnABC->setText("ㆍ");
    ui->btnDEF->setText("ㅡ");
    ui->btnGHI->setText("ㄱㅋ");
    ui->btnJKL->setText("ㄴㄹ");
    ui->btnMNO->setText("ㄷㅌ");
    ui->btnPQRS->setText("ㅂㅍ");
    ui->btnTUV->setText("ㅅㅎ");
    ui->btnWXYZ->setText("ㅈㅊ");
    ui->btnMark->setText("ㅇㅁ");
    ui->btnShift->setText(".,?!");
}


void MainWindow::on_btnEnglish_clicked()
{
    Korean = false;
    Number = false;

    resetKoreanState();

    lastButton = nullptr;
    currentIndex = 0;
    englishTimer->stop();

    shift_EnglishButtons();

    ui->btnMark->setText(".,?!");
    ui->btnSpecialMark->setText("@#/&_");
    ui->btnShift->setText("⇧"); // Enter문자 LLM 사용
}
*/

void MainWindow::on_btnLanguage_clicked()
{
    if (!Korean)
    {
        // 영어 -> 한글
        Korean = true;
        Number = false;

        lastButton = nullptr;
        currentIndex = 0;
        englishTimer->stop();

        resetKoreanState();

        ui->btnSpecialMark->setText("ㅣ");
        ui->btnABC->setText("ㆍ");
        ui->btnDEF->setText("ㅡ");
        ui->btnGHI->setText("ㄱㅋ");
        ui->btnJKL->setText("ㄴㄹ");
        ui->btnMNO->setText("ㄷㅌ");
        ui->btnPQRS->setText("ㅂㅍ");
        ui->btnTUV->setText("ㅅㅎ");
        ui->btnWXYZ->setText("ㅈㅊ");
        ui->btnMark->setText("ㅇㅁ");
        ui->btnShift->setText(".,?!");
    }
    else
    {
        // 한글 -> 영어
        Korean = false;
        Number = false;

        resetKoreanState();

        lastButton = nullptr;
        currentIndex = 0;
        englishTimer->stop();

        shift_EnglishButtons();

        ui->btnMark->setText(" ");
        ui->btnSpecialMark->setText(".,?!");
        ui->btnShift->setText("⇧");
    }

    updateLanguageButton();
}

void MainWindow::on_btnNumber_clicked()
{
    // 둘 다 true 되는 것을 방지
    Korean = false;
    Number = true;

    resetKoreanState();

    ui->btnSpecialMark->setText("1(-)");
    ui->btnABC->setText("2$");
    ui->btnDEF->setText("3^%#");
    ui->btnGHI->setText("4{*}");
    ui->btnJKL->setText("5+×÷");
    ui->btnMNO->setText("6<=>");
    ui->btnPQRS->setText("7[:]");
    ui->btnTUV->setText(" 8\"'; ");
    ui->btnWXYZ->setText("9/\\|");
    ui->btnMark->setText("0~.");
    ui->btnShift->setText(".,?!");
}

void MainWindow::updateLanguageButton()
{
    if (Korean)
    {
        languageLabel->setText(
            "<span style='font-size:18px; font-weight:700; color:black;'>한</span>"
            "<span style='font-size:18px; color:#AAAAAA;'> / 영</span>"
            );
    }
    else
    {
        languageLabel->setText(
            "<span style='font-size:18px; color:#AAAAAA;'>한 / </span>"
            "<span style='font-size:18px; font-weight:700; color:black;'>영</span>"
            );
    }
}

void MainWindow::on_btnComma_clicked()
{
    if (Korean)
        inputKoreanConsonant(ui->btnComma, "ㅈㅊㅉ");

    else if(Number)
        inputEnglishKey(ui->btnComma, "9/\\|");

    else
        inputEnglishKey(ui->btnComma, ",");
}

