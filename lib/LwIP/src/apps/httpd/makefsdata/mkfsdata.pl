#!/usr/bin/perl

open(OUTPUT, "> fsdata.c");

chdir("fs");
open(FILES, "find . -type f |");


print(OUTPUT "#include \"fs.h\"\n");
print(OUTPUT "#include \"lwip/def.h\"\n");
print(OUTPUT "#include \"fsdata.h\"\n\n");

print(OUTPUT "#define file_NULL (struct fsdata_file *) NULL\n\n");


while($file = <FILES>) {

    # Do not include files in CVS directories nor backup files.
    if($file =~ /(CVS|~)/) {
    	next;
    }

    chop($file);

    open(HEADER, "> /tmp/header") || die $!;

    if($file =~ /404/) {
	   print(HEADER "HTTP/1.0 404 File not found\r\n");
    } else {
	   print(HEADER "HTTP/1.0 200 OK\r\n");
    }

    print(HEADER "Server: lwIP/1.4.1 (stm32f746)\r\n");

    if($file =~ /\.html$/) {
	   print(HEADER "Content-type: text/html\r\n");
    } elsif($file =~ /\.shtml$/) {
       print(HEADER "Content-type: text/html\r\n");
    } elsif($file =~ /\.gif$/) {
	   print(HEADER "Content-type: image/gif\r\n");
    } elsif($file =~ /\.png$/) {
	   print(HEADER "Content-type: image/png\r\n");
    } elsif($file =~ /\.jpg$/) {
	   print(HEADER "Content-type: image/jpeg\r\n");
    } elsif($file =~ /\.class$/) {
	   print(HEADER "Content-type: application/octet-stream\r\n");
    } elsif($file =~ /\.ram$/) {
	   print(HEADER "Content-type: audio/x-pn-realaudio\r\n");    
    } else {
	   print(HEADER "Content-type: text/plain\r\n");
    }

    print(HEADER "\r\n");
    close(HEADER);

    unless($file =~ /\.plain$/ || $file =~ /cgi/) {
	   system("cat /tmp/header $file > /tmp/file");
    } else {
	   system("cp $file /tmp/file");
    }

    open(FILE, "/tmp/file");

    unlink("/tmp/file");
    unlink("/tmp/header");

    $file =~ s/\.//;
    $fvar = $file;
    $fvar =~ s-/-_-g;
    $fvar =~ s-\.-_-g;

    print(OUTPUT "static const unsigned char data".$fvar."[] = {\n");
    print(OUTPUT "\t/* $file */\n\t");

    for($j = 0; $j < length($file); $j++) {
	   printf(OUTPUT "0x%02x, ", unpack("C", substr($file, $j, 1)));
    }
    printf(OUTPUT "0,\n");

    $i = 0;
    while(read(FILE, $data, 1)) {
        if($i == 0) {
            print(OUTPUT "\t");
        }
        printf(OUTPUT "0x%02x, ", unpack("C", $data));
        $i++;
        if($i == 16) {
            print(OUTPUT "\n");
            $i = 0;
        }
    }
    print(OUTPUT "};\n\n");
    close(FILE);
    push(@fvars, $fvar);
    push(@files, $file);
}

for($i = 0; $i < @fvars; $i++) {
    $file = $files[$i];
    $fvar = $fvars[$i];

    if($i == 0) {
        $prevfile = "file_NULL";
    } else {
        $prevfile = "file" . $fvars[$i - 1];
    }
    print(OUTPUT "const struct fsdata_file file".$fvar."[] = {{\n\t$prevfile, \n\tdata$fvar,\n\t");
    print(OUTPUT "data$fvar + ". (length($file) + 1) .",\n\t");
    print(OUTPUT "sizeof(data$fvar) - ". (length($file) + 1) .",\n\t1\n\t}};\n\n");
}

print(OUTPUT "#define FS_ROOT file$fvars[$i - 1]\n\n");
print(OUTPUT "#define FS_NUMFILES $i\n");
