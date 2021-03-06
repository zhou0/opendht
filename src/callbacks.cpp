#include "callbacks.h"

namespace dht {


GetCallbackSimple
bindGetCb(GetCallbackRaw raw_cb, void* user_data)
{
    if (not raw_cb) return {};
    return [=](const std::shared_ptr<Value>& value) {
        return raw_cb(value, user_data);
    };
}

GetCallback
bindGetCb(GetCallbackSimple cb)
{
    if (not cb) return {};
    return [=](const std::vector<std::shared_ptr<Value>>& values) {
        for (const auto& v : values)
            if (not cb(v))
                return false;
        return true;
    };
}

ShutdownCallback
bindShutdownCb(ShutdownCallbackRaw shutdown_cb_raw, void* user_data)
{
    return [=]() { shutdown_cb_raw(user_data); };
}

DoneCallback
bindDoneCb(DoneCallbackSimple donecb)
{
    if (not donecb) return {};
    using namespace std::placeholders;
    return std::bind(donecb, _1);
}

DoneCallback
bindDoneCb(DoneCallbackRaw raw_cb, void* user_data)
{
    if (not raw_cb) return {};
    return [=](bool success, const std::vector<std::shared_ptr<Node>>& nodes) {
        raw_cb(success, (std::vector<std::shared_ptr<Node>>*)&nodes, user_data);
    };
}

DoneCallbackSimple
bindDoneCbSimple(DoneCallbackSimpleRaw raw_cb, void* user_data) {
    if (not raw_cb) return {};
    return [=](bool success) {
        raw_cb(success, user_data);
    };
}

std::string
NodeStats::toString() const
{
    std::stringstream ss;
    ss << "Known nodes: " << good_nodes << " good, " << dubious_nodes << " dubious, " << incoming_nodes << " incoming." << std::endl;
    if (table_depth > 1) {
        ss << "Routing table depth: " << table_depth << std::endl;
        unsigned long tot_nodes = 8 * std::exp2(table_depth);
        ss << "Network size estimation: " << tot_nodes << " nodes" << std::endl;
    }
    return ss.str();
}

}
