<?php
// Define the Car class
class BeSign {
    // Properties
    public $type;
    public $pdf_path;
    public $isProtected;
    public $image_path;
    public $output_path;
    public $p12Path;
    public $nik;
    public $passphraseBSrE;
    public $passphraseCert;
    public $page;
    public $x;
    public $y;
    public $width;
    public $height;
    public $id;
    public $secret;
    public $isLTV;
    public $isSeal;

    // Constructor
    public function __construct($type, $pdf_path, $isProtected, $char, $image_path, $output_path, $p12Path, $nik, $passphraseBSrE, $passphraseCert, $page, $x, $y, $width, $height, $id, $secret, $isLTV, $isSeal) {
        $this->type = $type;
        $this->pdf_path = $pdf_path;
        $this->isProtected = $isProtected;
        $this->char = $char;
        $this->image_path = $image_path;
        $this->output_path = $output_path;
        $this->p12Path = $p12Path;
        $this->nik = $nik;
        $this->passphraseBSrE = $passphraseBSrE;
        $this->passphraseCert = $passphraseCert;
        $this->page = $page;
        $this->x = $x;
        $this->y = $y;
        $this->width = $width;
        $this->height = $height;
        $this->id = $id;
        $this->secret = $secret;
        $this->isLTV = $isLTV;
        $this->isSeal = $isSeal;
    }



    private function get_token($id, $secret) {
        // Initialize cURL session
        $curl = curl_init();
        if (!$curl) {
            echo "Failed to initialize cURL.";
            return "";
        }

        // Set the URL for the API endpoint
        $url = "https://api-bsre.bssn.go.id/oauth2/token";
        curl_setopt($curl, CURLOPT_URL, $url);

        // Set Basic authentication credentials
        $auth = $id . ":" . $secret;
        curl_setopt($curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
        curl_setopt($curl, CURLOPT_USERPWD, $auth);

        // Set the request method to POST
        curl_setopt($curl, CURLOPT_POST, 1);

        // Set request headers
        $headers = array(
            'Content-Type: application/json'
        );
        curl_setopt($curl, CURLOPT_HTTPHEADER, $headers);

        // Set the request body
        $bodyData = json_encode(array(
            "grant_type" => "client_credentials"
        ));
        curl_setopt($curl, CURLOPT_POSTFIELDS, $bodyData);

        // Set the option to receive the response
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);

        // Perform the request
        $response = curl_exec($curl);
        if ($response === false) {
            echo "Failed to perform request: " . curl_error($curl);
            curl_close($curl);
            return "";
        }

        // Check HTTP status code
        $http_code = curl_getinfo($curl, CURLINFO_HTTP_CODE);
        if ($http_code !== 200) {
            echo "Error: HTTP status code $http_code";
            curl_close($curl);
            return "";
        }

        // Close cURL session
        curl_close($curl);

        // Parse JSON response
        $json_response = json_decode($response, true);
        if ($json_response === null) {
            echo "Error parsing JSON";
            return "";
        }

        // Extract access token
        if (isset($json_response['access_token'])) {
            return $json_response['access_token'];
        } else {
            echo "Access token not found in response";
            return "";
        }
    }

    private function get_signature($bsHash, $nik, $passphrase, $id, $secret, $token) {

        // Initialize cURL
        $url = "https://api-bsre.bssn.go.id/v2/esign/v2/api/entity/sign/hash/pdf";
        $ch = curl_init($url);

        // Create the JSON object
        $jsonData = '{
            "nik": "'.$nik.'",
            "idSubscriber": null,
            "passphrase": "'.$passphrase.'",
            "signaturePropertiesHashMap": {
                "0": {
                    "imageBase64": null,
                    "tampilan": "INVISIBLE",
                    "fieldId": null,
                    "page": 0,
                    "originX": 0.0,
                    "originY": 0.0,
                    "width": 0.0,
                    "height": 0.0,
                    "signatureLevel": "PAdES_BASELINE_LT",
                    "certificationPermission": "CHANGES_PERMITTED",
                    "signingDate": "2022-06-24T12:12:23.649+0000",
                    "certificateChainBase64": null,
                    "location": null,
                    "reason": null,
                    "contactInfo": null,
                    "documentDigestBase64": "'.$bsHash.'"
                }
            }
        }';  // JSON data to send

        // Set the POST request options
        curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'POST');
        curl_setopt($ch, CURLOPT_POSTFIELDS, $jsonData);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_HTTPHEADER, array(
            'Content-Type: application/json',
            'Authorization: Bearer ' . $token,
            'Content-Length: ' . strlen($jsonData)
        ));

        // Execute the request
        $responseData = curl_exec($ch);

        // Check for errors
        if ($responseData === false) {
            $signature = "Failed to perform request";
            error_log("Error: " . curl_error($curl));
            return get_signature($bsHash, $nik, $passphrase, $id, $secret, $token);
        } else {
            try {
                // Parse JSON string
                $j = json_decode($responseData, true);
                $cmsSignedDataHashMap = $j['data']['cmsSignedDataHashMap'];

                if (isset($cmsSignedDataHashMap['0']) && $cmsSignedDataHashMap['0'] !== null) {
                    // Get data with key
                    $sgn = $cmsSignedDataHashMap['0'];
                    $signature = $sgn;

                    return $signature;

                } else {
                    // Handle case where "signature" is null or not present
                    $signature = "Error: Signature data missing or null";
                    error_log("Error: Signature data missing or null");
                    $new_token = get_token($id, $secret);
                    $file_path = './token.json';
                    $data = array(
                        'token' => $new_token,
                        'timestamp' => time(), // Optionally add more data
                    );
                    $json_data = json_encode($data, JSON_PRETTY_PRINT);
                    $file_handle = fopen($file_path, 'w');
                    fwrite($file_handle, $json_data);
                    fclose($file_handle);
                    return get_signature($bsHash, $nik, $passphrase, $id, $secret, $new_token);
                }

            } catch (Exception $e) {
                $signature = "Error parsing JSON";
                error_log("Error parsing JSON: " . $e->getMessage());
                return "";
            }
        }

        // Close cURL session
        curl_close($ch);

    }

    // Method to display car details
    public function sign() {

        $ffi = FFI::cdef("
        char* placeHolder(int type, const char* pdf_path, int isProtected, const char* character, const char* imageorurl, int page, float x, float y, float width, float height, int isSeal);
        void free_string(const char* buffer);
        char* place(const char* pdf_char, const char* signature_char, const char* catalog_char, const char* byteRange0_char, const char* byteRange1_char, const char* byteRange2_char, const char* byteRange3_char, const char* outputPath);
        void signWithP12(int type, const char* pdf_path, int isProtected, const char* character, const char* imageorurl, const char* output_path, int page, float x, float y, float width, float height, const char* p12Path, const char* passphrase, int isSeal);
        void signBSrE(int type, const char* pdf_path, int isProtected, const char* character, const char* imageorurl, const char* output_path, int page, float x, float y, float width, float height, const char* nik, const char* passphrase, const char* id, const char* secret, int isLTV, int isSeal);
        ", "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/build/lib/libbesign.dylib");

        $type = $this->type;
        $pdf_path = $this->pdf_path;
        $isProtected = $this->isProtected;
        $char = $this->char;
        $image_path = $this->image_path;
        $output_path = $this->output_path;
        $p12Path = $this->p12Path;
        $nik = $this->nik;
        $passphraseBSrE = $this->passphraseBSrE;
        $passphraseCert = $this->passphraseCert;
        $page = $this->page;
        $x = $this->x;
        $y = $this->y;
        $width = $this->width;
        $height = $this->height;
        $id = $this->id;
        $secret = $this->secret;
        $isLTV = $this->isLTV;
        $isSeal = $this->isSeal;

        // $result = $ffi->placeHolder($type, $pdf_path, $isProtected, $char, $image_path, $page, $x, $y, $width, $height, $isSeal);
        $ffi->signWithP12($type, $pdf_path, $isProtected, $char, $image_path, $output_path, $page, $x, $y, $width, $height, $p12Path, $passphraseCert, $isSeal);
        // $ffi->signBSrE($type, $pdf_path, $isProtected, $char, $image_path, $output_path, $page, $x, $y, $width, $height, $nik, $passphraseBSrE, $id, $secret, $isLTV, $isSeal);


        // $php_string = FFI::string($result);

        // $data_array = json_decode($php_string, true);
        // $hash = $data_array['hash'];
        // $catalog = $data_array['catalog'];
        // $pdfs_hex = $data_array['pdfs'];
        // $byterange0 = $data_array['byterange0'];
        // $byterange1 = $data_array['byterange1'];
        // $byterange2 = $data_array['byterange2'];
        // $byterange3 = $data_array['byterange3'];
        // $pdfs = hex2bin($pdfs_hex);

        // // Example usage:
        // $file_path = './token.json';

        // if (file_exists($file_path)) {
        //     $json_data = file_get_contents($file_path);
        //     $data = json_decode($json_data, true); // Decode JSON string to PHP array

        //     // Check if decoding was successful
        //     if ($data !== null) {
        //         $token = $data['token'];
        //         $timestamp = $data['timestamp'];
        //         $signature = $this->get_signature($hash, $nik, $passphraseBSrE, $id, $secret, $token);
        //     } else {
        //         $token = $this->get_token($id, $secret);
        //         $signature = $this->get_signature($hash, $nik, $passphraseBSrE, $id, $secret, $token);
        //         $data = array(
        //             'token' => $token,
        //             'timestamp' => time(), // Optionally add more data
        //         );
        //         $json_data = json_encode($data, JSON_PRETTY_PRINT);
        //         $file_handle = fopen($file_path, 'w');
        //         fwrite($file_handle, $json_data);
        //         fclose($file_handle);
        //     }
        // } else {
        //     $token = $this->get_token($id, $secret);
        //     $signature = $this->get_signature($hash, $nik, $passphraseBSrE, $id, $secret, $token);
        //     $data = array(
        //         'token' => $token,
        //         'timestamp' => time(), // Optionally add more data
        //     );
        //     $json_data = json_encode($data, JSON_PRETTY_PRINT);
        //     $file_handle = fopen($file_path, 'w');
        //     fwrite($file_handle, $json_data);
        //     fclose($file_handle);
        // }

        // $ffi->place($pdfs_hex, $signature, $catalog, $byterange0, $byterange1, $byterange2, $byterange3, $output_path);

        // $pdf_fix = $ffi->place($pdfs_hex, $signature, $catalog, $byterange0, $byterange1, $byterange2, $byterange3, $output_path);
        // $p = FFI::string($pdf_fix);
        // $pdf = hex2bin($p);

        // file_put_contents($output_path, $pdf);
    }
}
?>