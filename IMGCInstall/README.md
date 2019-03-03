# ImageCash
Shell script to install a [ImageCash Masternode](https://www.imgcash.imagehosty.com/) on a Linux server running Ubuntu 18.04 or higher. Use it on your own risk.

***
## Installation:
```
git clone https://github.com/mceme/ImageCash.git
cd ImageCash
cd IMGCInstall
bash IMGCscript.sh
```
***

## Desktop wallet setup

After the MN is up and running, you need to configure the desktop wallet accordingly. Here are the steps for Windows Wallet
1. Open the imgcash Coin Desktop Wallet.
2. Go to RECEIVE and create a New Address: **MN1**
3. Send **1000** **imgcash** to **MN1**.
4. Wait for 15 confirmations.
5. Go to **Tools -> "Debug console - Console"**
6. Type the following command: **masternode outputs**
7. Go to  ** Tools -> "Open Masternode Configuration File"
8. Add the following entry:
```
Alias Address Privkey TxHash Output_index
```
* Alias: **MN1**
* Address: **VPS_IP:PORT**
* Privkey: **Masternode Private Key**
* TxHash: **First value from Step 6**
* Output index:  **Second value from Step 6**
9. Save and close the file.
10. Go to **Masternode Tab**. If you tab is not shown, please enable it from: **Settings - Options - Wallet - Show Masternodes Tab**
11. Click **Update status** to see your node. If it is not shown, close the wallet and start it again. Make sure the wallet is unlocked.
12. Open **Debug Console** and type:
```
masternode start-alias "MN1"
```
***

## Usage:
```
imgcash-cli getinfo
imgcash-cli mnsync status
imgcash-cli masternode status
```
Also, if you want to check/start/stop **Imagecash** , run one of the following commands as **root**:

**Ubuntu 18.04**:
```
systemctl status Imgcash #To check the service is running.
systemctl start Imgcash #To start Imagecash service.
systemctl stop Imgcash #To stop Imagecash service.
systemctl is-enabled Imgcash #To check whetether Imagecash service is enabled on boot or not.
```
***
