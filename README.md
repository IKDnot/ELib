# ELib

* ESP32、M5Stack向け自作汎用ライブラリ置き場
* センサ、アクチュエータを制御する EJ_XXX クラスと EJ_XXX クラスを管理する EJ_XXX_Manager クラスを提供
* EJ_XXX_Managerクラスによって実世界のオブジェクトとソフトウェア内のインスタンスをstrictに紐づけ、管理可能

## 仕様

* [API仕様書](https://ikdnot.github.io/ELib/annotated.html)

## Platform.io下での環境構築

1. 任意のProjectを作成する (例: example_proj)
1. `cd example_proj/lib` でlib直下に移動する
2. `git submodule add -b lib https://github.com/IKDnot/ELib` でElibをサブモジュールとしてインポートする
3. example_proj/platform.ini の lib_depsに `Elib` を追加する
   
   ```yaml
   [env:m5stack-core2]
    platform = espressif32
    board = m5stack-core2
    framework = arduino
    lib_deps = 
	m5stack/M5Core2@^0.1.5
	madhephaestus/ESP32Servo@^0.12.1
    Elib ; ←これを追記する
    ```

4. 使いたいファイルでファイルをインクルードする
   
   ```c
   #include <Elib.h>
   ```

## 基本的な使い方

1. ファイルをインクルードする
   
   ```c
   #include <Elib.h>
   ```
1. マネージャクラスのインスタンスを生成する(初期化)
   
    ```c
    const size_t MAX_MOTOR_NUM = 4;
    EJ_DCMotor_Manager::configure(MAX_MOTOR_NUM);
    ```

1. 制御クラスのインスタンスをマネージャクラス経由で生成する
   
   ```c
   EJ_DCMotor* motor = NULL;
   motor = EJ_DCMotor_Manager::createMotor(13, 14, 0); /* ピン1, ピン2, インスタンス識別id */
   motorAlias = EJ_DCMotor_Manager::getMotor(0);
   /* motorとmotorAliasは同一のポインタを返します */
   ```

1. メソッドを呼び出す

    ```c
    motor->forward(); /* モーターを正転させる */
    delay(1000);
    motorAlias->stop(); /* モーターを停止させる */
    ```

## DEBUG MODE

platformio.ini のbuild_flagにM5_DEBUGを定義することで M5系列の開発ボードでエラーログを出力可能です。

```yaml
[env:m5stack-core2]
platform = espressif32
board = m5stack-core2
framework = arduino
lib_deps = 
	m5stack/M5Core2@^0.1.5
	madhephaestus/ESP32Servo@^0.12.1
build_flags = ; ←これを追記する
  -D M5_DEBUG ; ←これを追記する
```


