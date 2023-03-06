/**
 * @file           EJ_DCMotor.h
 * @brief          DCモーターを制御するクラスEJ_DCMotorクラスと、EJ_DCMotorクラスを管理するEJ_DCMotor_Managerクラスの定義
 * @author         IKDnot
 * @date           2023/03/05
 * 
 * License
 * 
 * Copyright (c) 2023 IKDnot
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EJDCMOTOR
#define EJDCMOTOR
#include <Arduino.h>

/**
 * @struct MotorDef
 * @brief 1つのDCモータを定義する構造体
 */
typedef struct
{
    uint8_t pin1; /**< モーターの接続ピン1 */
    uint8_t pin2; /**< モーターの接続ピン2 */
    uint8_t id;   /**< モーターの識別番号 */
} MotorDef;

/**
 * @brief DCモーターを制御するクラス
 * @details *注意:本クラスのインスタンスはEJ_DCMotor_Managerクラス以外からは生成できない
 */
class EJ_DCMotor
{
private:
    /**
     * @brief EJ_DCMotorクラスのコンストラクタ
     * @param pin1 モーターの接続ピン1
     * @param pin2 モーターの接続ピン2
     */
    EJ_DCMotor(uint8_t pin1 = 0, uint8_t pin2 = 0);

    friend class EJ_DCMotor_Manager;

private:
    /**
     * @brief モーターにbit値を書き込むの関数
     * @param bit1 モーターの接続ピン1に書き込む値 (HIGH/LOW)
     * @param bit2 モーターの接続ピン2に書き込む値 (HIGH/LOW)
     */
    void write(uint8_t bit1, uint8_t bit2);

public:
    /**
     * @brief EJ_DCMotor クラスのデストラクタ
     */
    ~EJ_DCMotor();

public:
    /**
     * @brief モーターを正転させる
     */
    void forward();

    /**
     * @brief モーターを逆転させる
     */
    void reverse();

    /**
     * @brief モーターを停止させる
     */
    void stop();

private:
    static const char* _classname;
    uint8_t _pin1;
    uint8_t _pin2;
};

/**
 * @brief EJ_DCMotorクラスのインスタンスを生成、管理するクラス
 * @details 本クラスはシングルトンで提供され、getInstanceメソッドにのみよってインスタンス生成される
 */
class EJ_DCMotor_Manager
{
private:
    /**
     * @brief EJ_DCMotor_Managerクラスのコンストラクタ
     * @param maxInstanceSize EJ_DCMotorの最大インスタンス数の定義
     */
    EJ_DCMotor_Manager(size_t maxInstanceSize);

    /**
     * @brief EJ_DCMotor_Manager のインスタンスを取得する
     * @attention ＊一度もEJ_DCMotor_Manager::getInstance(size_t maxInstanceSize)をコールしていない場合はNULLポインタを返す
     * @return EJ_DCMotor_Managerのsingletonを指すポインタ
     */
    static EJ_DCMotor_Manager* getInstance();

public:
    /**
     * @brief EJ_DCMotor_Managerのインスタンスを生成する
     * @param maxInstanceSize EJ_DCMotorの最大インスタンス数の定義
     * @return true: 生成成功 / false: 生成失敗
     */
    static bool configure(size_t maxInstanceSize);

    /**
     * @brief EJ_DCMotorクラスのインスタンスを生成する
     * @details 生成したインスタンスはgetMotor関数で取得できるように同時に自身の_instanceList配列に記憶しておく
     * @param MotorDef 参照
     * @return EJ_DCMotorクラスのインスタンスを指すポインタ
     */
    static EJ_DCMotor *createMotor(MotorDef motor);

    /**
     * @brief EJ_DCMotorクラスのインスタンスを生成する
     * @details 生成したインスタンスはgetMotor関数で取得できるように同時に自身の_instanceList配列に記憶しておく
     * @param pin1 モータの接続ピン1
     * @param pin2 モータの接続ピン2
     * @param id モーターの識別番号
     * @return EJ_DCMotorクラスのインスタンスを指すポインタ
     */
    static EJ_DCMotor *createMotor(uint8_t pin1, uint8_t pin2, uint8_t id);

    /**
     * @brief EJ_DCMotorクラスのインスタンスを取得する
     * @details createMotor関数で生成したEJ_DCMotorクラスのインスタンスを取得し返す。指定したidのインスタンスが生成されていない場合や取得に失敗した場合はNULLを返す。
     * @param MotorDef 取得したいDCモータを指すDCモータ定義構造体
     * @return EJ_DCMotorクラスのインスタンスを指すポインタ
     */
    static EJ_DCMotor *getMotor(MotorDef motor);

    /**
     * @brief EJ_DCMotorクラスのインスタンスを取得する
     * @details createMotor関数で生成したEJ_DCMotorクラスのインスタンスを取得し返す。指定したidのインスタンスが生成されていない場合や取得に失敗した場合はNULLを返す。
     * @param id モーターの識別番号
     * @return EJ_DCMotorクラスのインスタンスを指すポインタ
     */
    static EJ_DCMotor *getMotor(uint8_t id);

public:
    /**
     * @brief EJ_DCMotor_Managerクラスのデストラクタ
     * @details 本クラスによって生成されたEJ_DCMotorクラスのインスタンスをすべて解放する
     */
    ~EJ_DCMotor_Manager();

private:
    static const char* _classname;
    static EJ_DCMotor_Manager *_singleton;
    const size_t _maxInstanceSize;
    EJ_DCMotor **_instanceList;
};

#endif // EJDCMOTOR