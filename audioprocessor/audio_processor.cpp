
#include "audio_processor.hpp"
#include <array>
#include <iostream>

CAP::AudioProcessor::AudioProcessor(StreamWriter * const streamWriters_, std::int8_t streamWriterCount_): streamWriters(streamWriters_), streamWriterCount(streamWriterCount_) {
    if (streamWriterCount == 0 || streamWriterCount > StreamWriterCapacity) {
        throw std::runtime_error("At least one stream writer, but no more than 5 writers is expected");
    }
    for (int i = 0; i < streamWriterCount; i++) {
        streamWriters[i].start();
    }
}

CAP::AudioProcessor::ProcessResult CAP::AudioProcessor::process(std::int16_t *samples, std::size_t nsamples) {
    
    auto result = ProcessResult::Success;

    
    for (int i = 0; i < streamWriterCount; i++) {
        auto qSize = streamWriters[i].queueSize();
        if (i == 0) {
            if (qSize >= streamWriterKillThreshold) {
                //priority writer has issues, kill all
                result = ProcessResult::PriorityWriterError;
                streamWriters[i].kill().get();
            } else {
                if (streamWriters[i].isWriteable()) {
                    streamWriters[i].enqueue(AudioBuffer(samples, nsamples));
                }
            }
        } else {
            //non-priority writer has issues, kill it
            if (qSize >= streamWriterKillThreshold || result == ProcessResult::PriorityWriterError) {
                streamWriters[i].kill().get();
                if (result != ProcessResult::PriorityWriterError) {
                    result = ProcessResult::NonPriorityWriterError;
                }
            } else {
                if (streamWriters[i].isWriteable()) {
                    streamWriters[i].enqueue(AudioBuffer(samples, nsamples));
                }
            }
        }
    }
    
    return result;
}

void CAP::AudioProcessor::stop() {
    for (int i = 0; i < streamWriterCount; i++) {
        streamWriters[i].stop().get();
    }
//    while (true) {
//        int i = 0;
//        for (auto &streamWriter: streamWriters) {
//            std::cout << i << "+:" << streamWriter.numberOfBuffersWritten() << " q:" << streamWriter.queueSize() << std::endl;
//            i += 1;
//        }
//        
//        
//        std::this_thread::sleep_for(std::chrono::milliseconds(10));
//        
//    }
}


void CAP::AudioProcessor::scheduleAudioPostProcessing(std::function<void ()> callback) {
//    auto priorityWrite = streamWriters[0]->numberOfBuffersWritten();
//    for(auto& streamWriter : streamWriters) {
//        if (streamWriter == streamWriters[0]) {
//            //skip priority writer
//            continue;
//        }
//        streamWriter->start();
//    }
//    std::async(std::launch::async, [](){
//        
//    });
//    callback();
}
