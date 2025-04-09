import React, { useContext, useEffect, useState } from "react";
import { configContext, requestProviderContext,queryProviderContext } from "../Context/Context";

function Home() {
  const [currentStatus, setCurrentStatus] = useContext(requestProviderContext);
  const [requestState, dispatchRequest] = useContext(queryProviderContext);
  const [clock, setClock] = useState(null);
  const [version, setVersion] = useState(null);

  useEffect(() => {
    if (currentStatus !== null) {
      setClock(currentStatus.currentTime);
      setVersion(currentStatus.version);
    }
  }, [currentStatus]);
      return (
        <div>
          <h2>Huidige status</h2>
          <div className="section">
            <div className="labelCell">
              <label>Klok </label>
            </div>
            <div className="smallBodyCell">{clock}</div>
          </div>
          <div className="section">
            <div className="labelCell">
              <label>Versie </label>
            </div>
            <div className="smallBodyCell">{version}</div>
          </div>
          </div>
      );
    }

    export default React.memo(Home);