/**
 * @file           EJ_ToFUnit.h
 * @brief          ToFセンサユニットVL53L0Xを制御するクラスEJ_ToFUnitクラスと、EJ_ToFUnitクラスを管理するEJ_ToFUnit_Managerクラスの定義
 * @author         IKDnot
 * @date           2023/03/08
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

#ifndef EJToFUnit
#define EJToFUnit
#include <Arduino.h>
#include <VL53L0X.h>

/**
 * @brief ToFセンサユニットを制御するクラス
 * @details *注意:本クラスのインスタンスはEJ_ToFUnit_Managerクラス以外からは生成できない
 */
class EJ_ToFUnit : public VL53L0X
{
private:
    /**
     * @brief EJ_ToFUnitクラスのコンストラクタ
     * @param address I2C通信で利用するアドレス
     */
    EJ_ToFUnit(uint8_t address);

    friend class EJ_ToFUnit_Manager;

public:
    /**
     * @brief EJ_ToFUnitクラスのデストラクタ
     */
    ~EJ_ToFUnit();

public:
    /**
     * @brief 距離を取得する (単位: mm)
     */
    virtual uint16_t read();

private:
    static const char* _classname;
    uint8_t _address;
    bool _error;
};

/**
 * @brief EJ_ToFUnitクラスのインスタンスを生成、管理するクラス
 * @details 本クラスはシングルトンで提供され、configure()メソッドにのみよってインスタンスが1つだけ生成される
 */
class EJ_ToFUnit_Manager
{
private:
    /**
     * @brief EJ_ToFUnit_Managerクラスのコンストラクタ
     * @param maxInstanceSize EJ_ToFUnitの最大インスタンス数の定義
     */
    EJ_ToFUnit_Manager(size_t maxInstanceSize);

private:
    /**
     * @brief EJ_ToFUnit_Manager のインスタンスを取得する
     * @attention *configure()がコールされていない場合はNULLポインタを返す
     * @return EJ_ToFUnit_Managerのsingletonを指すポインタ
     */
    static EJ_ToFUnit_Manager* getInstance();

public:
    /**
     * @brief EJ_ToFUnit_Managerクラスのデストラクタ
     * @details 本クラスによって生成されたEJ_ToFUnitクラスのインスタンスをすべて解放する
     */
    ~EJ_ToFUnit_Manager();

public:
    /**
     * @brief EJ_ToFUnit_Managerのインスタンスを生成する
     * @param maxInstanceSize EJ_ToFUnitの最大インスタンス数の定義
     * @return true: 生成成功 / false: 生成失敗
     */
    static bool configure(size_t maxInstanceSize);

    /**
     * @brief EJ_ToFUnitクラスのインスタンスを生成する
     * @details 生成したインスタンスはgetToFUnit関数で取得できるように同時に自身の_instanceList配列に記憶しておく
     * @param id ToFセンサユニットの識別番号
     * @return EJ_ToFUnitクラスのインスタンスを指すポインタ
     */
    static EJ_ToFUnit *createToFUnit(uint8_t id);

    /**
     * @brief EJ_ToFUnitクラスのインスタンスを取得する
     * @details createToFUnit関数で生成したEJ_ToFUnitクラスのインスタンスを取得し返す。指定したidのインスタンスが生成されていない場合や取得に失敗した場合はNULLを返す。
     * @param id ToFセンサユニットの識別番号
     * @return EJ_ToFUnitクラスのインスタンスを指すポインタ
     */
    static EJ_ToFUnit *getToFUnit(uint8_t id);

private:
    static const char* _classname;
    static EJ_ToFUnit_Manager *_singleton;
    const uint8_t _defaultAddress; // 0x29
    const size_t _maxInstanceSize;
    EJ_ToFUnit **_instanceList;
};

#endif // EJToFUnit