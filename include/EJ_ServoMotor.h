/**
 * @file           EJ_ServoMotor.h
 * @brief          サーボモーターを制御するクラスEJ_ServoMotorクラスと、EJ_ServoMotorクラスを管理するEJ_ServoMotor_Managerクラスの定義
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

    

#ifndef EJSERVOMOTOR
#define EJSERVOMOTOR
#include <Arduino.h>
#include <ESP32Servo.h>

/**
 * @struct ServoDef
 * @brief 1つのサーボモータを定義する構造体
 */
typedef struct
{
    uint8_t pin;    /**< モーターの接続ピン */
    uint8_t id;     /**< モーターの識別番号 */
} ServoDef;

/**
 * @brief サーボモーターを制御するクラス
 * @attention*本クラスのインスタンスはEJ_ServoMotor_Managerクラス以外からは生成できない
 * @details [ESP32Servo.h](https://github.com/jkb-git/ESP32Servo/blob/master/src/ESP32_Servo.h) で定義されるServoクラスを基底とした派生クラス
 */
class EJ_ServoMotor : public Servo
{
private:
    /**
     * @brief EJ_ServoMotorクラスのコンストラクタ
     * @param pin モーターの接続ピン
     * @param min サーボの角度が0度のときのパルス幅[us]。デフォルト = 544
     * @param max サーボの角度が180度のときのパルス幅[us]。デフォルト = 2400
     */
    EJ_ServoMotor(uint8_t pin = 0, int min = 544, int max = 2400);
    
    friend class EJ_ServoMotor_Manager;

public:
    /**
     * @brief EJ_ServoMotor クラスのデストラクタ
     */
    ~EJ_ServoMotor();

public:
    /**
     * @brief 指定した角度までサーボモータを回転させる
     * @param value 静止させたい角度 (0~180)
     */
    virtual void write(int angle);
    
    /**
     * @brief 出力をマイクロ秒単位で角度指定してサーボモータを回転させる。サーボモータに与えられるパルスは20ms周期で、1周期中のHigh時間を直接指定する。
     * @param us 出力したいパルス幅[us]
     */
    virtual void writeMicroseconds(int us); // Write pulse width in microseconds 
    
    /**
     * @brief 現在のサーボの角度(最後にwriteした値)を読み取る
     * @return angle 現在のサーボの角度
     */
    virtual int read();                        // returns current pulse width as an angle between 0 and 180 degrees

private:
    static const char* _classname;
    uint8_t _pin;
    int _min_;
    int _max_;
};

/**
 * @brief EJ_ServoMotorクラスのインスタンスを生成、管理するクラス
 * @details 本クラスはシングルトンで提供され、configure()メソッドにのみよってインスタンスが1つだけ生成される
 */
class EJ_ServoMotor_Manager
{
private:
    /**
     * @brief EJ_ServoMotor_Managerクラスのコンストラクタ
     * @param maxInstanceSize EJ_ServoMotorの最大インスタンス数の定義
     */
    EJ_ServoMotor_Manager(size_t maxInstanceSize);

private:
    /**
     * @brief EJ_ServoMotor_Manager のインスタンスを取得する
     * @attention *configure()がコールされていない場合はNULLポインタを返す
     * @return EJ_ServoMotor_Managerのsingletonを指すポインタ
     */
    static EJ_ServoMotor_Manager* getInstance();

public:
    /**
     * @brief EJ_ServoMotor_Managerクラスのデストラクタ
     * @details 本クラスによって生成されたEJ_ServoMotorクラスのインスタンスをすべて解放する
     */
    ~EJ_ServoMotor_Manager();

public:
    /**
     * @brief EJ_ServoMotor_Managerのインスタンスを生成する
     * @param maxInstanceSize EJ_ServoMotorの最大インスタンス数の定義
     * @return true: 生成成功 / false: 生成失敗
     */
    static bool configure(size_t maxInstanceSize);

    /**
     * @brief EJ_ServoMotorクラスのインスタンスを生成する
     * @details 生成したインスタンスはgetServo関数で取得できるように同時に自身の_instanceList配列に記憶しておく
     * @param ServoDef 参照
     * @return EJ_ServoMotorクラスのインスタンスを指すポインタ
     */
    static EJ_ServoMotor *createServo(ServoDef servo);

    /**
     * @brief EJ_ServoMotorクラスのインスタンスを生成する
     * @details 生成したインスタンスはgetServo関数で取得できるように同時に自身の_instanceList配列に記憶しておく
     * @param pin サーボモータの接続ピン
     * @param id サーボモータの識別番号
     * @param min サーボの角度が0度のときのパルス幅[us]。デフォルト = 544
     * @param max サーボの角度が180度のときのパルス幅[us]。デフォルト = 2400
     * @return EJ_ServoMotorクラスのインスタンスを指すポインタ
     */
    static EJ_ServoMotor *createServo(uint8_t pin, uint8_t id, int min = 544, int max = 2400);

    /**
     * @brief EJ_ServoMotorクラスのインスタンスを取得する
     * @details createServo関数で生成したEJ_ServoMotorクラスのインスタンスを取得し返す。指定したidのインスタンスが生成されていない場合や取得に失敗した場合はNULLを返す。
     * @param MotorDef 取得したいサーボモータを指すサーボモータ定義構造体
     * @return EJ_ServoMotorクラスのインスタンスを指すポインタ
     */
    static EJ_ServoMotor *getServo(ServoDef servo);

    /**
     * @brief EJ_ServoMotorクラスのインスタンスを取得する
     * @details createServo関数で生成したEJ_ServoMotorクラスのインスタンスを取得し返す。指定したidのインスタンスが生成されていない場合や取得に失敗した場合はNULLを返す。
     * @param id モーターの識別番号
     * @return EJ_ServoMotorクラスのインスタンスを指すポインタ
     */
    static EJ_ServoMotor *getServo(uint8_t id);

private:
    static const char* _classname;
    static EJ_ServoMotor_Manager *_singleton;
    const size_t _maxInstanceSize;
    EJ_ServoMotor **_instanceList;
};

#endif // EJSERVOMOTOR