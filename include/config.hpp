/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PTZController.h
 * Author: ha
 *
 * Created on August 23, 2017, 11:14 AM
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <sstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class IPConfig{
private:
    string cameraToken = "MainStream";
    string camUrl = "";
    static size_t CallbackFunc(void *contents, size_t size, size_t nmemb, std::string *s)
    {
        size_t newLength = size*nmemb;
        size_t oldLength = s->size();
        try
        {
            s->resize(oldLength + newLength);
        }
        catch(std::bad_alloc &e)
        {
            //handle memory problem
            return 0;
        }

        std::copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);
        return size*nmemb;
    }
public:    
    IPConfig(string ip)
    {camUrl = "http://" + ip + "/onvif/device_service";}
    
    string post(string content){
        std::string respondContent = "";
        CURL*curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, camUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackFunc);


            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respondContent);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

        }
        curl_global_cleanup();
        //cout << respondContent << endl; 
        return respondContent;
    }

    string getToken(){
        std::string respondContent = "";
        CURL*curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, camUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackFunc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respondContent);
            string strGetProfile = "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:a=\"http://www.w3.org/2005/08/addressing\"><s:Header><Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\"><UsernameToken><Username>admin</Username><Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">admin</Password><Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\"></Nonce><Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\"></Created></UsernameToken></Security></s:Header><s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"><GetProfiles xmlns=\"http://www.onvif.org/ver10/media/wsdl\"/></s:Body> </s:Envelope>";
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strGetProfile.c_str());
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            //std::cout<<"Returned:"<<respondContent;
        }
        return "";
    }
    
    enum Dir{
        Left,Right,Up,Down
    };

    void setGOP(int gop){
        string command = "<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" xmlns:tt=\"http://www.onvif.org/ver10/schema\"><soap:Body><trt:SetVideoEncoderConfiguration><trt:Configuration token=\"VideoEncoderConfiguration0\"><tt:Name>VideoEncoderConfiguration0</tt:Name><tt:UseCount>1</tt:UseCount><tt:Encoding>H264</tt:Encoding><tt:Resolution><tt:Width>1920</tt:Width><tt:Height>1080</tt:Height></tt:Resolution><tt:Quality>4</tt:Quality><tt:H264><tt:GovLength>" + MyIntToString(gop) + "</tt:GovLength><tt:H264Profile>Baseline</tt:H264Profile></tt:H264><tt:Multicast><tt:Address><tt:Type>IPv4</tt:Type><tt:IPv4Address>255.255.255.255</tt:IPv4Address></tt:Address><tt:Port>1234</tt:Port><tt:TTL>1</tt:TTL><tt:AutoStart>true</tt:AutoStart></tt:Multicast><tt:SessionTimeout>PT1H1M1S</tt:SessionTimeout></trt:Configuration><trt:ForcePersistence>true</trt:ForcePersistence></trt:SetVideoEncoderConfiguration></soap:Body></soap:Envelope>";
        post(command);
    }
    void setIRCut(bool onIR){
        string irCut = (onIR)?("ON"):("OFF");
        cout<<irCut<<endl;
        string command = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:c14n=\"http://www.w3.org/2001/10/xml-exc-c14n#\" xmlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\" xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" xmlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\" xmlns:wsrp=\"http://schemas.xmlsoap.org/rp/\" xmlns:wsa=\"http://www.w3.org/2005/08/addressing\" xmlns:wsrfbf2=\"http://docs.oasis-open.org/wsrf/bf-2\" xmlns:xmime=\"http://www.w3.org/2005/05/xmlmime\" xmlns:xop=\"http://www.w3.org/2004/08/xop/include\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" xmlns:wsrfr2=\"http://docs.oasis-open.org/wsrf/r-2\" xmlns:ONVIFCreatePullPoint=\"http://www.onvif.org/ver10/events/wsdl/CreatePullPointBinding\" xmlns:ONVIFEvent=\"http://www.onvif.org/ver10/events/wsdl/EventBinding\" xmlns:ONVIFNotificationConsumer=\"http://www.onvif.org/ver10/events/wsdl/NotificationConsumerBinding\" xmlns:ONVIFNotificationProducer=\"http://www.onvif.org/ver10/events/wsdl/NotificationProducerBinding\" xmlns:ONVIFPausableSubscriptionManager=\"http://www.onvif.org/ver10/events/wsdl/PausableSubscriptionManagerBinding\" xmlns:ONVIFPullPoint=\"http://www.onvif.org/ver10/events/wsdl/PullPointSubscriptionBinding\" xmlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" xmlns:ONVIFPullPoint2=\"http://www.onvif.org/ver10/events/wsdl/PullPointBinding\" xmlns:ONVIFSubscriptionManager=\"http://www.onvif.org/ver10/events/wsdl/SubscriptionManagerBinding\" xmlns:ONVIFanalytics=\"http://www.onvif.org/ver10/analytics/wsdl/AnalyticsEngineBinding\" xmlns:ONVIFanalytics2=\"http://www.onvif.org/ver20/analytics/wsdl/AnalyticsEngineBinding\" xmlns:ONVIFruleEngin2=\"http://www.onvif.org/ver20/analytics/wsdl/RuleEngineBinding\" xmlns:tan2=\"http://www.onvif.org/ver20/analytics/wsdl\" xmlns:ONVIFruleEngine=\"http://www.onvif.org/ver10/analytics/wsdl/RuleEngineBinding\" xmlns:tan=\"http://www.onvif.org/ver10/analytics/wsdl\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns:timg=\"http://www.onvif.org/ver10/imaging/wsdl\" xmlns:timg2=\"http://www.onvif.org/ver20/imaging/wsdl\" xmlns:tptz=\"http://www.onvif.org/ver10/ptz/wsdl\" xmlns:tptz2=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\"><SOAP-ENV:Header><wsse:Security SOAP-ENV:mustUnderstand=\"true\"><wsse:UsernameToken><wsse:Username>admin</wsse:Username><wsse:Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">1111</wsse:Password><wsse:Nonce>cU//ULiH1uuhjlugI4BFygV2Ot8=</wsse:Nonce><wsu:Created>2013-01-23T02:48:17Z</wsu:Created></wsse:UsernameToken></wsse:Security></SOAP-ENV:Header><SOAP-ENV:Body><timg2:SetImagingSettings><timg2:VideoSourceToken>VideoSource1</timg2:VideoSourceToken><timg2:ImagingSettings><tt:IrCutFilter>"+irCut+"</tt:IrCutFilter></timg2:ImagingSettings></timg2:SetImagingSettings></SOAP-ENV:Body></SOAP-ENV:Envelope>";
        post(command);

    }
    void resetWDR(){
        string command = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:c14n=\"http://www.w3.org/2001/10/xml-exc-c14n#\" xmlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\" xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" xmlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\" xmlns:wsrp=\"http://schemas.xmlsoap.org/rp/\" xmlns:wsa=\"http://www.w3.org/2005/08/addressing\" xmlns:wsrfbf2=\"http://docs.oasis-open.org/wsrf/bf-2\" xmlns:xmime=\"http://www.w3.org/2005/05/xmlmime\" xmlns:xop=\"http://www.w3.org/2004/08/xop/include\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" xmlns:wsrfr2=\"http://docs.oasis-open.org/wsrf/r-2\" xmlns:ONVIFCreatePullPoint=\"http://www.onvif.org/ver10/events/wsdl/CreatePullPointBinding\" xmlns:ONVIFEvent=\"http://www.onvif.org/ver10/events/wsdl/EventBinding\" xmlns:ONVIFNotificationConsumer=\"http://www.onvif.org/ver10/events/wsdl/NotificationConsumerBinding\" xmlns:ONVIFNotificationProducer=\"http://www.onvif.org/ver10/events/wsdl/NotificationProducerBinding\" xmlns:ONVIFPausableSubscriptionManager=\"http://www.onvif.org/ver10/events/wsdl/PausableSubscriptionManagerBinding\" xmlns:ONVIFPullPoint=\"http://www.onvif.org/ver10/events/wsdl/PullPointSubscriptionBinding\" xmlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" xmlns:ONVIFPullPoint2=\"http://www.onvif.org/ver10/events/wsdl/PullPointBinding\" xmlns:ONVIFSubscriptionManager=\"http://www.onvif.org/ver10/events/wsdl/SubscriptionManagerBinding\" xmlns:ONVIFanalytics=\"http://www.onvif.org/ver10/analytics/wsdl/AnalyticsEngineBinding\" xmlns:ONVIFanalytics2=\"http://www.onvif.org/ver20/analytics/wsdl/AnalyticsEngineBinding\" xmlns:ONVIFruleEngin2=\"http://www.onvif.org/ver20/analytics/wsdl/RuleEngineBinding\" xmlns:tan2=\"http://www.onvif.org/ver20/analytics/wsdl\" xmlns:ONVIFruleEngine=\"http://www.onvif.org/ver10/analytics/wsdl/RuleEngineBinding\" xmlns:tan=\"http://www.onvif.org/ver10/analytics/wsdl\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns:timg=\"http://www.onvif.org/ver10/imaging/wsdl\" xmlns:timg2=\"http://www.onvif.org/ver20/imaging/wsdl\" xmlns:tptz=\"http://www.onvif.org/ver10/ptz/wsdl\" xmlns:tptz2=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\"><SOAP-ENV:Header><wsse:Security SOAP-ENV:mustUnderstand=\"true\"><wsse:UsernameToken><wsse:Username>user1</wsse:Username><wsse:Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">RK+Od97N3hqeRb2hsUdZ3mS/V5M=</wsse:Password><wsse:Nonce>cU//ULiH1uuhjlugI4BFygV2Ot8=</wsse:Nonce><wsu:Created>2013-01-23T02:48:17Z</wsu:Created></wsse:UsernameToken></wsse:Security></SOAP-ENV:Header><SOAP-ENV:Body><timg2:SetImagingSettings><timg2:VideoSourceToken>VideoSource1</timg2:VideoSourceToken><timg2:ImagingSettings><WideDynamicRange xmlns=\"http://www.onvif.org/ver10/schema\"><Mode>ON</Mode><Level>60</Level></WideDynamicRange></timg2:ImagingSettings></timg2:SetImagingSettings></SOAP-ENV:Body></SOAP-ENV:Envelope>";
        post(command);
    }
    std::string MyIntToString ( int Number )
    {
      std::ostringstream ss;
      ss << Number;
      return ss.str();
    }

};

void flushVideo(VideoCapture &vid){
    int fps = 1;
    int maxDelay = 1000/fps - 15;
    int delay = 0;
    int frameDelayCnt = 0;
    while (frameDelayCnt <= 1){
        auto start = std::chrono::high_resolution_clock::now();
        vid.grab();
        auto stop = std::chrono::high_resolution_clock::now();
        auto int_ms = stdd::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        delay = int_ms.count();
        cout << "Delay: " << delay << endl;
        if (delay > maxDelay)
            frameDelayCnt++;
    }
}

#endif /* PTZCONTROLLER_H */

