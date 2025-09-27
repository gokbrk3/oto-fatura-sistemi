$folder = "C:\projeler\oto-fatura-sistemi\indirilen_faturalar"   # klasör yolunu buraya yaz
$files = Get-ChildItem -Path $folder -Filter *.pdf

foreach ($file in $files) {
    Start-Process -FilePath $file.FullName -Verb Print
}
